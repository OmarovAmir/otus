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
#include <Options.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Connection
{
    static const auto delimetr = '\n';

    SafeQueuePtr<DataPtr> m_input_queue;
    SafeQueuePtr<DataPtr> m_output_queue;
    std::atomic_bool m_processedThreadFinish;
    std::thread m_processedThread;
    
    DataProcessor m_processor;

    std::shared_ptr<std::condition_variable> m_removeCV;
    tcp::socket m_input_socket;
    boost::asio::streambuf m_input_buffer;
    tcp::socket m_output_socket;
    boost::asio::streambuf m_output_buffer;
    asio::ip::tcp::resolver m_resolver;
    bool m_connectedInput;
    bool m_connectedOutput;

    tcp::endpoint inputEndpoint;
    tcp::endpoint outputEndpoint;

    void sendProcessed()
    {
        while (!m_processedThreadFinish)
        {
            auto data = m_output_queue->pop();
            if(data)
            {

                if(!data->IsEmpty())
                {
                    switch (data->GetDirection())
                    {
                        case DataDirection::ToClient:
                            if(LOG_INFO)
                            {
                                fmt::println("From server {}:{} to client {}:{} [{}]", outputEndpoint.address().to_string(),
                                        outputEndpoint.port(), inputEndpoint.address().to_string(),
                                        inputEndpoint.port(), data->GetData());
                            }
                            inputWrite(data->GetData() + delimetr);
                            break;
                        
                        case DataDirection::ToServer:
                            if(LOG_INFO)
                            {
                                fmt::println("From client {}:{} to server {}:{} [{}]", inputEndpoint.address().to_string(),
                                        inputEndpoint.port(), outputEndpoint.address().to_string(),
                                        outputEndpoint.port(), data->GetData());
                            }
                            outputWrite(data->GetData() + delimetr);
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
        : m_input_queue{std::make_shared<SafeQueue<DataPtr>>()}
        , m_output_queue{std::make_shared<SafeQueue<DataPtr>>()}
        , m_processedThreadFinish{false}
        , m_processedThread{&Connection::sendProcessed, this}
        , m_processor{m_input_queue, m_output_queue}
        , m_removeCV{removeCV}
        , m_input_socket{std::move(socket)}
        , m_input_buffer{}
        , m_output_socket{m_input_socket.get_executor()}
        , m_output_buffer{}
        , m_resolver{m_input_socket.get_executor()}
        , m_connectedInput{true}
        , m_connectedOutput{false}
        , inputEndpoint{m_input_socket.remote_endpoint()}
        , outputEndpoint{m_input_socket.local_endpoint()}
    {
        m_output_socket.open(tcp::v4());
        ip_transparent opt(true);
        m_output_socket.set_option(opt);
        m_output_socket.bind(m_input_socket.remote_endpoint());
        m_processedThread.detach();
    }

    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection()
    {
        m_processedThreadFinish = true;
        m_output_queue->clear();
        m_output_queue->stop();
    }

    void connect()
    {
        printConnection("Connection");
        m_output_socket.async_connect(m_input_socket.local_endpoint(),
                                      [this](const boost::system::error_code error) { handleConnect(error); });
    }

    void disconnect(boost::system::error_code error = boost::system::error_code())
    {
        printConnection("Disconnection");
        m_connectedInput = false;
        m_connectedOutput = false;
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
        m_removeCV->notify_one();
    }

    bool isConnected() { return m_connectedInput || m_connectedOutput; }

    void printConnection(std::string action)
    {
        if(isConnected() && LOG_INFO)
        {
            fmt::println("");
            fmt::println("{}", action);
            fmt::print("client: {}:{} ", inputEndpoint.address().to_string(),
                        inputEndpoint.port());
            fmt::println("server: {}:{}", outputEndpoint.address().to_string(),
                            outputEndpoint.port());
            fmt::println("");
        }
    }

private:
    void handleInputRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            if(LOG_ERROR)
            {
                fmt::println("{}: {}", __FUNCTION__, error.message());
            }
            disconnect(error);
        }
        else
        {
            std::string data{asio::buffer_cast<const char*>(m_input_buffer.data()), length};
            boost::trim(data);
            m_input_buffer.consume(length);
            m_input_queue->push(std::make_shared<Data>(DataDirection::FromClient, data));
            inputRead();
        }
    }

    void handleOutputRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            if(LOG_ERROR)
            {
                fmt::println("{}: {}", __FUNCTION__, error.message());
            }
            disconnect(error);
        }
        else
        {
            std::string data{asio::buffer_cast<const char*>(m_output_buffer.data()), length};
            boost::trim(data);
            m_output_buffer.consume(length);
            m_input_queue->push(std::make_shared<Data>(DataDirection::FromServer, data));
            outputRead();
        }
    }

    void handleInputWrite(const boost::system::error_code error, [[maybe_unused]] const std::size_t length)
    {
        if (error)
        {
            if(LOG_ERROR)
            {
                fmt::println("{}: {}", __FUNCTION__, error.message());
            }
            disconnect(error);
        }
    }

    void handleOutputWrite(const boost::system::error_code error, [[maybe_unused]] const std::size_t length)
    {
        if (error)
        {
            if(LOG_ERROR)
            {
                fmt::println("{}: {}", __FUNCTION__, error.message());
            }
            disconnect(error);
        }
    }

    void handleConnect(const boost::system::error_code error)
    {
        if (error)
        {
            if(LOG_ERROR)
            {
                fmt::println("{}: {}", __FUNCTION__, error.message());
            }
            disconnect(error);
        }
        else
        {
            m_connectedOutput = true;
            inputRead();
            outputRead();
        }
    }

    void inputRead()
    {
        if(m_connectedInput)
        {
           asio::async_read_until(m_input_socket, m_input_buffer, delimetr,
                               [this](const boost::system::error_code error, const std::size_t length)
                               { handleInputRead(error, length); });
        }
    }

    void outputRead()
    {
        if(m_connectedOutput)
        {
            asio::async_read_until(m_output_socket, m_output_buffer, delimetr,
                                [this](const boost::system::error_code error, const std::size_t length)
                                { handleOutputRead(error, length); });
        }
    }

    void inputWrite(const std::string& data)
    {
        if(m_connectedInput)
        {
            asio::async_write(m_input_socket, asio::buffer(data.data(), data.size()),
                            [this](const boost::system::error_code error, const std::size_t length)
                            { handleInputWrite(error, length); });
        }
    }

    void outputWrite(const std::string& data)
    {
        if(m_connectedOutput)
        {
            asio::async_write(m_output_socket, asio::buffer(data.data(), data.size()),
                            [this](const boost::system::error_code error, const std::size_t length)
                            { handleOutputWrite(error, length); });
        }
    }
};