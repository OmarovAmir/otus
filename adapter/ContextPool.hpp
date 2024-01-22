#pragma once

#include <vector>
#include <memory>
#include <thread>

#include <boost/asio.hpp>
namespace asio = boost::asio;

class ContextPool
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

     std::size_t contextNumber;
     std::size_t nextContext;
     std::vector<std::shared_ptr<ioContext>> ioContexts;
public:
     ContextPool(std::size_t contextNumber)
     : contextNumber{(0 != contextNumber) ? contextNumber : 1}
     , nextContext{0}
     , ioContexts{}
     {}
     ContextPool(const ContextPool&) = delete;
     ContextPool(ContextPool&&) = delete;
     ~ContextPool(){}

     asio::io_context& getNext()
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
          return ctx->getContext();
     }

     void stopAll()
     {
          for(auto& ctx: ioContexts)
          {
               ctx->getContext().stop();
          }
     }

     void joinAll()
     {
          for (auto& ctx: ioContexts)
          {
               ctx->join();
          }
     }
};