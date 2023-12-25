#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <Connection.hpp>
#include <async.hpp>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Server : public std::enable_shared_from_this<Server>
{
    std::size_t m_general;
    std::size_t m_size;
    asio::io_context m_ioContext;
    tcp::acceptor m_acceptor;
    asio::signal_set m_signals;
    std::vector<std::thread> m_threads;

    void accept()
    {
        auto self = clone();
        m_acceptor.async_accept(
            [self](const boost::system::error_code error, tcp::socket socket)
            {
                if (!error)
                {
                    const auto connection =
                        std::make_shared<Connection>(std::move(socket), self->m_size, self->m_general);
                    connection->read();
                    self->accept();
                }
            });
    }

    explicit Server(std::size_t port, std::size_t size)
        : m_general{connect(size)}
        , m_size{size}
        , m_ioContext{}
        , m_acceptor{m_ioContext, tcp::endpoint(tcp::v4(), port)}
        , m_signals{m_ioContext, SIGINT, SIGTERM}
    {}
    Server(const Server&) = delete;
    Server(Server&&) = delete;

  public:
    ~Server() { disconnect(m_general); }

    static std::shared_ptr<Server> create(std::size_t port, std::size_t size)
    {
        return std::shared_ptr<Server>(new Server(port, size));
    }

    std::shared_ptr<Server> clone() { return shared_from_this(); }

    void run()
    {
        auto self = clone();
        m_signals.async_wait(
            [self](auto, auto)
            {
                self->m_signals.clear();
                self->m_acceptor.cancel();
                self->m_acceptor.close();
                self->m_ioContext.stop();
            });
        accept();
        const auto nThreads = std::thread::hardware_concurrency();
        m_threads.reserve(nThreads);
        for (unsigned int i = 0; i < nThreads; ++i)
        {
            m_threads.emplace_back([self]() { self->m_ioContext.run(); });
        }
        for (auto& th : m_threads)
        {
            th.join();
        }
    }
};