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
        auto self = shared_from_this();
        m_acceptor.async_accept(
            [self](const boost::system::error_code error, tcp::socket socket)
            {
                self->accept();
                if (!error)
                {
                    const auto connection = std::make_shared<Connection>(std::move(socket), self->m_size, self->m_general);
                    connection->read();
                }
            });
    }

  public:
    explicit Server(std::size_t port, std::size_t size)
        : m_general{connect(size)}
        , m_size{size}
        , m_ioContext{}
        , m_acceptor{m_ioContext, tcp::endpoint(tcp::v4(), port)}
        , m_signals{m_ioContext, SIGINT, SIGTERM}
    {}
    Server(const Server&) = delete;
    Server(Server&&) = delete;
    ~Server() { disconnect(m_general); }

    void run()
    {
        m_signals.async_wait([this](auto, auto) { m_ioContext.stop(); });
        accept();
        const auto nThreads = std::thread::hardware_concurrency();
        m_threads.reserve(nThreads);
        for (unsigned int i = 0; i < nThreads; ++i)
        {
            m_threads.emplace_back([this]() { m_ioContext.run(); });
        }
        for (auto& th : m_threads)
        {
            th.join();
        }
    }
};