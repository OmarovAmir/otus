#pragma once

#include <iostream>
#include <memory>

#include <CommandType.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection>
{
    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    static const auto delimetr = '\n';

    void handleRead(const boost::system::error_code error, const std::size_t length)
    {
        if (!error)
        {
            if (length != 0)
            {
                std::string data{asio::buffer_cast<const char*>(m_buffer.data()), length};
                boost::trim(data);
                m_buffer.consume(length);
                std::cout << data << std::endl;
            }
            read();
        }
    }

  public:
    explicit Connection(tcp::socket socket)
        : m_socket{std::move(socket)}
    {}
    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection() {}

    void read()
    {
        auto self = shared_from_this();
        asio::async_read_until(m_socket, m_buffer, delimetr,
                               [self](const boost::system::error_code error, const std::size_t length)
                               { self->handleRead(error, length); });
    }
};