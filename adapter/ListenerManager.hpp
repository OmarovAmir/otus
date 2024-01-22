#pragma once

#include <vector>
#include <memory>
#include <thread>

#include <boost/asio/io_context.hpp>
#include <Listener.hpp>

namespace asio = boost::asio;

class ListenerManager
{
     class ioContext
     {
          asio::io_context ctx;
          asio::executor_work_guard<asio::io_context::executor_type> guard;
          std::thread ctxThread;
     public:
          explicit ioContext()
          : ctx{}
          , guard{ctx.get_executor()}
          , ctxThread{[this](){ctx.run();}}
          {}
          void join()
          {
               if(ctxThread.joinable())
               {
                    ctxThread.join();
               }
          }
          asio::io_context& getContext()
          {
               return ctx;
          }
     };

     std::size_t nextContext;
     std::size_t contextNumber;
     std::vector<std::shared_ptr<ioContext>> ioContexts;
     std::vector<std::size_t> ports;
     std::vector<std::shared_ptr<Listener>> listeners;
     asio::io_context signalsCtx;
     asio::signal_set signals;

     asio::io_context& getNextContext()
     {
          std::shared_ptr<ioContext> ctx;
          if(ioContexts.size() < contextNumber)
          {
               ctx = std::make_shared<ioContext>();
               ioContexts.push_back(ctx);
          }
          else
          {
               ctx = ioContexts[nextContext++];
               nextContext %= contextNumber;
          }
          std::size_t x = reinterpret_cast<std::size_t>(ctx.get());
          fmt::println("{}", x);
          return ctx->getContext();
     }

     void runAllListeners()
     {
          for (const auto& port: ports)
          {
               auto listener = std::make_shared<Listener>(getNextContext(), port);
               listeners.push_back(listener);
               listener->run();
          }
          joinAllContexts();
     }

     void stopAllListeners()
     {
          for(auto& listener: listeners)
          {
               listener->stop();
          }
     }

     void stopAllContexts()
     {
          for(auto& ctx: ioContexts)
          {
               ctx->getContext().stop();
          }
     }
     
     void joinAllContexts()
     {
          for (auto& ctx: ioContexts)
          {
               ctx->join();
          }
     }

public:
     ListenerManager(std::size_t ioContextNumber, 
     std::vector<std::size_t> ports)
     : nextContext{0}
     , contextNumber{(0 != ioContextNumber) ? ioContextNumber : 1}
     , ioContexts{}
     , ports{ports}
     , listeners{}
     , signals{getNextContext(), SIGINT, SIGTERM}
     {}
     ListenerManager(const ListenerManager&) = delete;
     ListenerManager(ListenerManager&&) = delete;
     ~ListenerManager(){}

     void stop()
     {
          signals.clear();
          stopAllListeners();
          stopAllContexts();
     }
     
     void run()
     {
          signals.async_wait(
               [this](auto, auto)
               {
                    stop();
               });
          runAllListeners();
     }
};
