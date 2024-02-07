#pragma once

#include <iostream>
#include <memory>
#include <condition_variable>

#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>
#include <fmt/format.h>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

#include <netinet/in.h>

class Connection
{
    static const auto delimetr = '\n';

    using ip_transparent = boost::asio::detail::socket_option::boolean<SOL_IP, IP_TRANSPARENT>;

    std::shared_ptr<std::condition_variable> m_removeCV;
    tcp::socket m_input_socket;
    boost::asio::streambuf m_input_buffer;
    tcp::socket m_output_socket;
    boost::asio::streambuf m_output_buffer;
    asio::ip::tcp::resolver m_resolver;
    bool m_connected;

    void printSocketsEndpoints()
    {
        fmt::println("Connection");
        fmt::print("Input socket: ");
        fmt::print("remote: {} {} ", m_input_socket.remote_endpoint().address().to_string(), m_input_socket.remote_endpoint().port());
        fmt::println("local: {} {}", m_input_socket.local_endpoint().address().to_string(), m_input_socket.local_endpoint().port());
        fmt::print("Output socket: ");
        fmt::print("remote: {} {} ", m_output_socket.remote_endpoint().address().to_string(), m_output_socket.remote_endpoint().port());
        fmt::println("local: {} {}", m_output_socket.local_endpoint().address().to_string(), m_output_socket.local_endpoint().port());
        fmt::println("");
    }

    void handleInputRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            fmt::println("{}", error.message());
            m_connected = false;
            m_removeCV->notify_one();
        }
        else
        {
            std::string data{asio::buffer_cast<const char*>(m_input_buffer.data()), length};
            boost::trim(data);
            m_input_buffer.consume(length);
            if(data.size())
            {
                fmt::println("input read: {}", data);
                outputWrite("HAHAHAHA" + data);
            }
            inputRead();
        }
    }

    void handleOutputRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            fmt::println("{}", error.message());
            m_connected = false;
            m_removeCV->notify_one();
        }
        else
        {
            std::string data{asio::buffer_cast<const char*>(m_output_buffer.data()), length};
            boost::trim(data);
            m_output_buffer.consume(length);
            if(data.size())
            {
                fmt::println("output read: {}", data);
                size_t pos = data.find("HAHAHAHA");
                if( pos != std::string::npos)
                {
                    data.erase(pos, std::string("HAHAHAHA").size());
                }
                inputWrite(data);
            }
            outputRead();
        }
    }

    void handleInputWrite(const boost::system::error_code error, [[maybe_unused]] const std::size_t length)
    {
        if (error)
        {
            fmt::println("{}", error.message());
            m_connected = false;
            m_removeCV->notify_one();
        }
    }

    void handleOutputWrite(const boost::system::error_code error, [[maybe_unused]] const std::size_t length)
    {
        if (error)
        {
            fmt::println("{}", error.message());
            m_connected = false;
            m_removeCV->notify_one();
        }
    }

    void handleConnect(const boost::system::error_code error)
    {
        if (error)
        {
            fmt::println("{}", error.message());
            m_connected = false;
            m_removeCV->notify_one();
        }
        else
        {
            printSocketsEndpoints();
            inputRead();
            outputRead();
            m_connected = true;
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
        asio::async_write(m_input_socket, asio::buffer(data.data(), data.size()),
                          [this](const boost::system::error_code error, const std::size_t length)
                          { handleInputWrite(error, length); });
    }

    void outputWrite(std::string data)
    {
        asio::async_write(m_output_socket, asio::buffer(data.data(), data.size()),
                          [this](const boost::system::error_code error, const std::size_t length)
                          { handleOutputWrite(error, length); });
    }

  public:
    explicit Connection(tcp::socket socket, std::shared_ptr<std::condition_variable> removeCV)
        : m_removeCV{removeCV}
        , m_input_socket{std::move(socket)}
        , m_input_buffer{}
        , m_output_socket{m_input_socket.get_executor()}
        , m_output_buffer{}
        , m_resolver{m_input_socket.get_executor()}
        , m_connected{true}
    {
        m_output_socket.open(tcp::v4());
        ip_transparent opt(true);
        m_output_socket.set_option(opt);
        m_output_socket.bind(m_input_socket.remote_endpoint());
    }

    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection() {}

    void connect()
    {
        m_output_socket.async_connect(m_input_socket.local_endpoint(),
            [this](const boost::system::error_code error)
                            { handleConnect(error); });
    }

    void disconnect()
    {
        m_input_socket.close();
        m_output_socket.close();
    }

    bool isConnected()
    {
        return m_connected;
    }
};