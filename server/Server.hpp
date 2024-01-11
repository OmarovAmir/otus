#pragma once

#include <iostream>
#include <memory>
#include <thread>
#include <vector>

#include <Connection.hpp>
#include <boost/asio.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Server : public std::enable_shared_from_this<Server>
{
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
                    const auto connection = std::make_shared<Connection>(std::move(socket));
                    connection->read();
                    self->accept();
                }
            });
    }

    explicit Server(std::size_t port)
        : m_ioContext{}
        , m_acceptor{m_ioContext, tcp::endpoint(tcp::v4(), port)}
        , m_signals{m_ioContext, SIGINT, SIGTERM}
    {}
    Server(const Server&) = delete;
    Server(Server&&) = delete;

  public:
    ~Server() {}

    static std::shared_ptr<Server> create(std::size_t port)
    {
        return std::shared_ptr<Server>(new Server(port));
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