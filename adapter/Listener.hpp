#pragma once

#include <fmt/format.h>

#include <boost/asio.hpp>

#include <Connection.hpp>
#include <ConnectionManager.hpp>
#include <Logger.hpp>
#include <SocketIpTransparentOption.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Listener
{
    tcp::acceptor m_acceptor;
    ConnectionManager m_connectionManager;

    void handleAccept(const boost::system::error_code error, tcp::socket socket)
    {
        if (error)
        {
            auto logger = Logger::getInstance();
            logger.error(fmt::format("{}: {}", __PRETTY_FUNCTION__, error.message()));
        }
        else
        {
            m_connectionManager.createConnection(std::move(socket));
            accept();
        }
    }

    void accept()
    {
        m_acceptor.async_accept([this](const boost::system::error_code error, tcp::socket socket)
                                { handleAccept(error, std::move(socket)); });
    }

  public:
    explicit Listener(asio::io_context& context, std::size_t port)
        : m_acceptor{context, tcp::endpoint(tcp::v4(), port)}
        , m_connectionManager{}
    {
        ip_transparent opt(true);
        m_acceptor.set_option(opt);
    }
    Listener(const Listener&) = delete;
    Listener(Listener&&) = delete;
    ~Listener() {}

    void run() { accept(); }

    void stop() { m_acceptor.close(); }
};