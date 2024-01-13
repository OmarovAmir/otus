#pragma once

#include <iostream>
#include <memory>

#include <CommandType.hpp>
#include <async.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>
#include <fmt/format.h>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection>
{
    static const auto delimetr = '\n';

    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;

    std::shared_ptr<std::size_t> m_client;

    void handleRead(const boost::system::error_code error, const std::size_t length)
    {
        if (!error)
        {
            if (length != 0)
            {
                std::string data{asio::buffer_cast<const char*>(m_buffer.data()), length};
                boost::trim(data);
                m_buffer.consume(length);
                if (m_client)
                {
                    receive(*m_client, data);
                }
            }
            read();
        }
        else
        {
            fmt::println("Error acync read");
            if (m_client)
            {
                disconnect(*m_client);
                m_client.reset();
            }
        }
    }

    void handleWrite(const boost::system::error_code error, const std::size_t length)
    {
        if (!error)
        {
            if (length != 0)
            {
                fmt::println("Success acync write");
            }
        }
        else
        {
            fmt::println("Error acync write");
            if (m_client)
            {
                disconnect(*m_client);
                m_client.reset();
            }
        }
    }

  public:
    explicit Connection(tcp::socket socket)
        : m_socket{std::move(socket)}
        , m_buffer{}
        , m_client{nullptr}
    {}
    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection() {}

    void read()
    {
        auto self = shared_from_this();

        if (!m_client)
        {
            m_client =
                std::make_shared<std::size_t>(connect([self](const std::string& answer) { self->write(answer); }));
        }

        asio::async_read_until(m_socket, m_buffer, delimetr,
                               [self](const boost::system::error_code error, const std::size_t length)
                               { self->handleRead(error, length); });
    }

    void write(std::string data)
    {
        auto self = shared_from_this();
        asio::async_write(m_socket, asio::buffer(data.data(), data.size()),
                          [self](const boost::system::error_code error, const std::size_t length)
                          { self->handleWrite(error, length); });
    }
};