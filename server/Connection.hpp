#pragma once

#include <iostream>
#include <memory>

#include <async.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/asio.hpp>
#include <CommandType.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection>
{
    tcp::socket m_socket;
    boost::asio::streambuf m_buffer;
    std::size_t m_client;
    std::size_t m_general;
    std::size_t m_current;
    static const auto delimetr = '\n';

    void handleRead(const boost::system::error_code error, const std::size_t length)
    {
        if (error)
        {
            std::cout << std::this_thread::get_id() << " Client \"" << m_client << "\": Reading error: \"" << error
                      << "\"." << std::endl;
        }
        else
        {
            if (length != 0)
            {
                static const std::string _levelUp{"{"};
                static const std::string _levelDown{"}"};

                std::string data{asio::buffer_cast<const char*>(m_buffer.data()), length};
                boost::trim(data);
                m_buffer.consume(length);
                if (_levelUp == data)
                {
                    m_current = m_client;
                }
                receive(m_current, data.data(), data.size());
                if (_levelDown == data)
                {
                    m_current = m_general;
                }
            }
            read();
        }
    }

  public:
    explicit Connection(tcp::socket socket, std::size_t size, std::size_t general)
        : m_socket{std::move(socket)}
        , m_client{connect(size)}
        , m_general{general}
        , m_current{general}
    {}
    Connection(const Connection&) = delete;
    Connection(Connection&&) = delete;
    ~Connection()
    {
        disconnect(m_client);
    }

    void read()
    {
        auto self = shared_from_this();
        asio::async_read_until(m_socket, m_buffer, delimetr,
                               [self](const boost::system::error_code error, const std::size_t length)
                               { self->handleRead(error, length); });
    }
};