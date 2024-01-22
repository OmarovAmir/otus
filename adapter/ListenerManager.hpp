#pragma once

#include <vector>
#include <memory>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <Listener.hpp>
#include <ContextPool.hpp>

namespace asio = boost::asio;

class ListenerManager
{
     ContextPool contextPool;
     std::vector<std::size_t> ports;
     std::vector<std::shared_ptr<Listener>> listeners;
     asio::signal_set signals;

     void runAll()
     {
          for (const auto& port: ports)
          {
               auto listener = std::make_shared<Listener>(contextPool.getNext(), port);
               listeners.push_back(listener);
               listener->run();
          }
          contextPool.joinAll();
     }

     void stopAll()
     {
          signals.clear();
          for(auto& listener: listeners)
          {
               listener->stop();
          }
          contextPool.stopAll();
     }

public:
     ListenerManager(std::size_t contextNumber, 
     std::vector<std::size_t> ports)
     : contextPool{contextNumber}
     , ports{ports}
     , listeners{}
     , signals{contextPool.getNext(), SIGINT, SIGTERM}
     {
          signals.async_wait(
               [this](auto, auto)
               {
                    stop();
               });
     }
     ListenerManager(const ListenerManager&) = delete;
     ListenerManager(ListenerManager&&) = delete;
     ~ListenerManager(){}

     void stop()
     {
          stopAll();
     }
     
     void run()
     {
          runAll();
     }
};
