#pragma once

#include <fmt/format.h>

#include <boost/asio.hpp>

#include <Connection.hpp>
#include <ConnectionManager.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

#include <netinet/in.h>

class Listener
{
     tcp::acceptor m_acceptor;
     ConnectionManager m_connectionManager;

     using ip_transparent = boost::asio::detail::socket_option::boolean<SOL_IP, IP_TRANSPARENT>;

     void accept()
     {
          m_acceptor.async_accept(
          [this](const boost::system::error_code error, tcp::socket socket)
          {
               if (error)
               {
                    fmt::println("{}", error.message());
               }
               else
               {
                    m_connectionManager.createConnection(std::move(socket));
                    accept();
               }
          });
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

     void run()
     {
          accept();
     }

     void stop()
     {
          m_acceptor.close();
     }
};