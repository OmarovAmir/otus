#pragma once

#include <iostream>
#include <memory>
#include <condition_variable>

#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>
#include <fmt/format.h>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Connection
{
    static const auto delimetr = '\n';

    std::shared_ptr<std::condition_variable> m_removeCV;
    tcp::socket m_input_socket;
    boost::asio::streambuf m_input_buffer;
    tcp::socket m_output_socket;
    boost::asio::streambuf m_output_buffer;
    asio::ip::tcp::resolver m_resolver;
    bool m_connected;

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
            if (length != 0)
            {
                std::string data{asio::buffer_cast<const char*>(m_input_buffer.data()), length};
                boost::trim(data);
                m_input_buffer.consume(length);
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
            if (length != 0)
            {
                std::string data{asio::buffer_cast<const char*>(m_output_buffer.data()), length};
                boost::trim(data);
                m_output_buffer.consume(length);
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

    void handleConnect(const boost::system::error_code error, asio::ip::tcp::resolver::iterator result)
    {
        if (error)
        {
            fmt::println("{}", error.message());
            m_connected = false;
            m_removeCV->notify_one();
        }
        else
        {
            fmt::println("connected to {} {}", result->endpoint().address().to_string(), result->endpoint().port());
            inputRead();
            outputRead();
            m_connected = true;
        }
    }

    void handlerResolve(const boost::system::error_code& error, asio::ip::tcp::resolver::iterator result)
    {
        if (error)
        {
            fmt::println("{}", error.message());
            m_connected = false;
            m_removeCV->notify_one();
        }
        else
        {
            asio::ip::tcp::resolver::iterator end;
            asio::async_connect(m_output_socket, result, end,  
                            [this](const boost::system::error_code error, asio::ip::tcp::resolver::iterator result)
                            { handleConnect(error, result); });
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
    {}

    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection() {}

    void connect()
    {
        m_resolver.async_resolve(m_input_socket.local_endpoint(), 
            [this](const boost::system::error_code& error, asio::ip::tcp::resolver::iterator result)
            {handlerResolve(error, result);});
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