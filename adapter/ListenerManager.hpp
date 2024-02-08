#pragma once

#include <list>
#include <memory>
#include <thread>
#include <vector>

#include <ContextPool.hpp>
#include <Listener.hpp>
#include <boost/asio/io_context.hpp>

namespace asio = boost::asio;

class ListenerManager
{
    ContextPool m_contextPool;
    std::vector<std::size_t> m_ports;
    std::list<std::shared_ptr<Listener>> m_listeners;
    asio::signal_set m_signals;

    void runAll()
    {
        for (const auto& port : m_ports)
        {
            auto listener = std::make_shared<Listener>(m_contextPool.getNext(), port);
            m_listeners.push_back(listener);
            listener->run();
        }
        m_contextPool.joinAll();
    }

    void stopAll()
    {
        m_signals.clear();
        for (auto& listener : m_listeners)
        {
            listener->stop();
        }
        m_contextPool.stopAll();
    }

  public:
    ListenerManager(std::size_t contextNumber, std::vector<std::size_t> ports)
        : m_contextPool{contextNumber}
        , m_ports{ports}
        , m_listeners{}
        , m_signals{m_contextPool.getNext(), SIGINT, SIGTERM}
    {
        m_signals.async_wait([this](auto, auto) { stop(); });
    }
    ListenerManager(const ListenerManager&) = delete;
    ListenerManager(ListenerManager&&) = delete;
    ~ListenerManager() {}

    void stop() { stopAll(); }

    void run() { runAll(); }
};
