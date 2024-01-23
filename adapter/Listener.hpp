#pragma once

#include <fmt/format.h>

#include <boost/asio.hpp>

#include <Connection.hpp>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

class Listener
{
     tcp::acceptor acceptor;

     void accept()
     {
          acceptor.async_accept(
          [this](const boost::system::error_code error, tcp::socket socket)
          {
               if (error)
               {
                    fmt::println("{}", error.message());
               }
               else
               {
                    fmt::print("remote: {} {} ", socket.remote_endpoint().address().to_string(), socket.remote_endpoint().port());
                    fmt::println("local: {} {}", socket.local_endpoint().address().to_string(), socket.local_endpoint().port());
                    const auto connection = std::make_shared<Connection>(std::move(socket));
                    connection->start();
                    accept();
               }
          });
     }

public:
     explicit Listener(asio::io_context& context, std::size_t port)
          : acceptor{context, tcp::endpoint(tcp::v4(), port)}
     {}
     Listener(const Listener&) = delete;
     Listener(Listener&&) = delete;
     ~Listener() {}

     void run()
     {
          accept();
     }

     void stop()
     {
          acceptor.close();
     }
};