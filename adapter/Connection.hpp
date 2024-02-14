#pragma once

#include <condition_variable>
#include <iostream>
#include <memory>

#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>
#include <fmt/format.h>

#include <SocketIpTransparentOption.hpp>
#include <DataProcessor.hpp>
#include <Data.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Connection
{
    static const auto delimetr = '\n';

    std::mutex m_processedMutex;
    std::shared_ptr<std::condition_variable> m_processedCV;
    bool m_processedThreadFinish;
    std::thread m_processedThread;

    DataProcessor m_processor;

    std::shared_ptr<std::condition_variable> m_removeCV;
    tcp::socket m_input_socket;
    boost::asio::streambuf m_input_buffer;
    tcp::socket m_output_socket;
    boost::asio::streambuf m_output_buffer;
    asio::ip::tcp::resolver m_resolver;
    bool m_connectedClient;
    bool m_connectedServer;

    void sendProcessed()
    {
        while (!m_processedThreadFinish)
        {
            std::unique_lock lock(m_processedMutex);
            m_processedCV->wait(lock, [this] { return m_processedThreadFinish || !m_processor.IsProcessedEmpty(); });
            auto data = m_processor.popAll();
            for (const auto& d: *data)
            {
                auto sd = d->GetData();
                if(!sd.empty())
                {
                    sd += delimetr;
                    switch (d->GetDirection())
                    {
                        case DataDirection::ToClient:
                            inputWrite(sd);
                            break;
                        
                        case DataDirection::ToServer:
                            outputWrite(sd);
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

public:
    explicit Connection(tcp::socket socket, std::shared_ptr<std::condition_variable> removeCV)
        : m_processedMutex{}
        , m_processedCV{std::make_shared<std::condition_variable>()}
        , m_processedThreadFinish{false}
        , m_processedThread{&Connection::sendProcessed, this}
        , m_processor{m_processedCV}
        , m_removeCV{removeCV}
        , m_input_socket{std::move(socket)}
        , m_input_buffer{}
        , m_output_socket{m_input_socket.get_executor()}
        , m_output_buffer{}
        , m_resolver{m_input_socket.get_executor()}
        , m_connectedClient{true}
        , m_connectedServer{false}
    {
        m_output_socket.open(tcp::v4());
        ip_transparent opt(true);
        m_output_socket.set_option(opt);
        m_output_socket.bind(m_input_socket.remote_endpoint());
    }

    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection()
    {
        {
            std::unique_lock lock(m_processedMutex);
            m_processedThreadFinish = true;
        }
        m_processedCV->notify_one();
        if (m_processedThread.joinable())
        {
            m_processedThread.join();
        }
    }

    void connect()
    {
        printConnection("Connection");
        m_output_socket.async_connect(m_input_socket.local_endpoint(),
                                      [this](const boost::system::error_code error) { handleConnect(error); });
    }

    void disconnect(boost::system::error_code error = boost::system::error_code())
    {
        if (error)
        {
            // fmt::println("{}", error.message());
        }
        {
            std::unique_lock lock(m_processedMutex);
            m_processor.Stop();
        }
        printConnection("Disconnection");
        if (m_input_socket.is_open())
        {
            m_input_socket.shutdown(tcp::socket::shutdown_both, error);
        }
        if (m_output_socket.is_open())
        {
            m_output_socket.shutdown(tcp::socket::shutdown_both, error);
        }
        m_input_socket.close();
        m_output_socket.close();
        m_connectedClient = false;
        m_connectedServer = false;
        m_removeCV->notify_one();
    }

    bool isConnected() { return m_connectedClient || m_connectedServer; }

    void printConnection(std::string action)
    {
        if (isConnected())
        {
            fmt::println("");
            fmt::println("{}", action);
            if (m_connectedClient)
            {
                fmt::print("client: {}:{} ", m_input_socket.remote_endpoint().address().to_string(),
                           m_input_socket.remote_endpoint().port());
                fmt::println("server: {}:{}", m_input_socket.local_endpoint().address().to_string(),
                             m_input_socket.local_endpoint().port());
            }
            fmt::println("");
        }
    }

private:
    void handleInputRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            disconnect(error);
        }
        else
        {
            std::string data{asio::buffer_cast<const char*>(m_input_buffer.data()), length};
            boost::trim(data);
            m_input_buffer.consume(length);
            auto pd = std::make_shared<Data>(DataDirection::FromClient, data);
            m_processor.push(pd);
            inputRead();
        }
    }

    void handleOutputRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            disconnect(error);
        }
        else
        {
            std::string data{asio::buffer_cast<const char*>(m_output_buffer.data()), length};
            boost::trim(data);
            m_output_buffer.consume(length);
            auto pd = std::make_shared<Data>(DataDirection::FromServer, data);
            m_processor.push(pd);
            outputRead();
        }
    }

    void handleInputWrite(const boost::system::error_code error, [[maybe_unused]] const std::size_t length)
    {
        if (error)
        {
            disconnect(error);
        }
    }

    void handleOutputWrite(const boost::system::error_code error, [[maybe_unused]] const std::size_t length)
    {
        if (error)
        {
            disconnect(error);
        }
    }

    void handleConnect(const boost::system::error_code error)
    {
        if (error)
        {
            disconnect(error);
        }
        else
        {
            inputRead();
            outputRead();
            m_connectedServer = true;
        }
    }

    void inputRead()
    {
        asio::async_read_until(m_input_socket, m_input_buffer, delimetr,
                               [this](const boost::system::error_code error, const std::size_t length)
                               { handleInputRead(error, length); });
    }

    void outputRead()
    {
        asio::async_read_until(m_output_socket, m_output_buffer, delimetr,
                               [this](const boost::system::error_code error, const std::size_t length)
                               { handleOutputRead(error, length); });
    }

    void inputWrite(std::string data)
    {
        fmt::println("From server {}:{} to client {}:{} [{}]", m_input_socket.local_endpoint().address().to_string(),
                     m_input_socket.local_endpoint().port(), m_input_socket.remote_endpoint().address().to_string(),
                     m_input_socket.remote_endpoint().port(), data);
        asio::async_write(m_input_socket, asio::buffer(data.data(), data.size()),
                          [this](const boost::system::error_code error, const std::size_t length)
                          { handleInputWrite(error, length); });
    }

    void outputWrite(std::string data)
    {
        fmt::println("From client {}:{} to server {}:{} [{}]", m_output_socket.local_endpoint().address().to_string(),
                     m_output_socket.local_endpoint().port(), m_output_socket.remote_endpoint().address().to_string(),
                     m_output_socket.remote_endpoint().port(), data);
        asio::async_write(m_output_socket, asio::buffer(data.data(), data.size()),
                          [this](const boost::system::error_code error, const std::size_t length)
                          { handleOutputWrite(error, length); });
    }
};