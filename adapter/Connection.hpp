#pragma once

#include <iostream>
#include <memory>

#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>
#include <fmt/format.h>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection>
{
    static const auto delimetr = '\n';

    tcp::socket input_socket;
    boost::asio::streambuf input_buffer;
    tcp::socket output_socket;
    boost::asio::streambuf output_buffer;

    void handleInputRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            fmt::println("{}", error.message());
        }
        else
        {
            if (length != 0)
            {
                std::string data{asio::buffer_cast<const char*>(input_buffer.data()), length};
                boost::trim(data);
                input_buffer.consume(length);
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
        }
        else
        {
            if (length != 0)
            {
                std::string data{asio::buffer_cast<const char*>(output_buffer.data()), length};
                boost::trim(data);
                output_buffer.consume(length);
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
        }
    }

    void handleOutputWrite(const boost::system::error_code error, [[maybe_unused]] const std::size_t length)
    {
        if (error)
        {
            fmt::println("{}", error.message());
        }
    }

    void handleConnect(const boost::system::error_code error)
    {
        if (error)
        {
            fmt::println("{}", error.message());
        }
        else
        {
            inputRead();
            outputRead();
        }
    }

  public:
    explicit Connection(tcp::socket socket)
        : input_socket{std::move(socket)}
        , input_buffer{}
        , output_socket{input_socket.get_executor()}
        , output_buffer{}
    {}

    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection() {}

    void inputRead()
    {
        auto self = shared_from_this();

        asio::async_read_until(input_socket, input_buffer, delimetr,
                               [self](const boost::system::error_code error, const std::size_t length)
                               { self->handleInputRead(error, length); });
    }

    void outputRead()
    {
        auto self = shared_from_this();

        asio::async_read_until(output_socket, output_buffer, delimetr,
                               [self](const boost::system::error_code error, const std::size_t length)
                               { self->handleOutputRead(error, length); });
    }

    void inputWrite(std::string data)
    {
        auto self = shared_from_this();
        asio::async_write(input_socket, asio::buffer(data.data(), data.size()),
                          [self](const boost::system::error_code error, const std::size_t length)
                          { self->handleInputWrite(error, length); });
    }

    void outputWrite(std::string data)
    {
        auto self = shared_from_this();
        asio::async_write(output_socket, asio::buffer(data.data(), data.size()),
                          [self](const boost::system::error_code error, const std::size_t length)
                          { self->handleOutputWrite(error, length); });
    }

    void start()
    {
        auto self = shared_from_this();
        output_socket.async_connect(/*input_socket.remote_endpoint()*/asio::ip::tcp::endpoint(input_socket.local_endpoint().address(), 4567),
                                [self](const boost::system::error_code error)
                                { self->handleConnect(error); });
    }
};