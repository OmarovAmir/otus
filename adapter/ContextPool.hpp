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
          asio::io_context m_ctx;
          asio::executor_work_guard<asio::io_context::executor_type> m_guard;
          std::thread m_ctxThread;
     public:
          explicit ioContext()
          : m_ctx{}
          , m_guard{m_ctx.get_executor()}
          , m_ctxThread{[this](){m_ctx.run();}}
          {}
          void join()
          {
               if(m_ctxThread.joinable())
               {
                    m_ctxThread.join();
               }
          }
          asio::io_context& getContext()
          {
               return m_ctx;
          }
     };

     std::size_t m_contextNumber;
     std::size_t m_nextContext;
     std::vector<std::shared_ptr<ioContext>> m_ioContexts;
public:
     ContextPool(std::size_t contextNumber)
     : m_contextNumber{(0 != contextNumber) ? contextNumber : 1}
     , m_nextContext{0}
     , m_ioContexts{}
     {}
     ContextPool(const ContextPool&) = delete;
     ContextPool(ContextPool&&) = delete;
     ~ContextPool(){}

     asio::io_context& getNext()
     {
          std::shared_ptr<ioContext> ctx;
          if(m_ioContexts.size() < m_contextNumber)
          {
               ctx = std::make_shared<ioContext>();
               m_ioContexts.push_back(ctx);
          }
          else
          {
               ctx = m_ioContexts[m_nextContext++];
               m_nextContext %= m_contextNumber;
          }
          return ctx->getContext();
     }

     void stopAll()
     {
          for(auto& ctx: m_ioContexts)
          {
               ctx->getContext().stop();
          }
     }

     void joinAll()
     {
          for (auto& ctx: m_ioContexts)
          {
               ctx->join();
          }
     }
};