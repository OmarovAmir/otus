#pragma once

#include <list>
#include <memory>
#include <algorithm>
#include <thread>
#include <condition_variable>
#include <mutex>

#include <boost/asio.hpp>

#include <fmt/format.h>

#include <Connection.hpp>

class ConnectionManager
{
     std::list<std::shared_ptr<Connection>> m_connections;
     std::mutex m_removeMutex;
     std::shared_ptr<std::condition_variable> m_removeCV;
     bool m_removeThreadFinish;
     std::thread m_removeThread;

     void removeDisconnectedConnections()
     {
          while(!m_removeThreadFinish)
          {
               std::unique_lock lock(m_removeMutex);
               m_removeCV->wait(lock, [this]{ return m_removeThreadFinish || !m_connections.empty(); });

               for (auto connection = m_connections.begin(); connection != m_connections.end();)
               {
                    if ((*connection)->isConnected())
                    {
                         ++connection;
                    }
                    else
                    {
                         (*connection)->printConnection("Disconnection");
                         connection = m_connections.erase(connection);
                    }
               }
          }
     }
     
public:
     ConnectionManager()
     : m_connections{}
     , m_removeMutex{}
     , m_removeCV{std::make_shared<std::condition_variable>()}
     , m_removeThreadFinish{false}
     , m_removeThread{&ConnectionManager::removeDisconnectedConnections, this}
     {}
     ConnectionManager(const ConnectionManager&) = delete;
     ConnectionManager(ConnectionManager&&) = delete;
     ~ConnectionManager()
     {
          {
               std::unique_lock lock(m_removeMutex);
               m_connections.clear();
               m_removeThreadFinish = true;
          }
          m_removeCV->notify_one();
          if(m_removeThread.joinable())
          {
               m_removeThread.join();
          }
     }

     void createConnection(boost::asio::ip::tcp::socket socket)
     {
          auto connection = std::make_shared<Connection>(std::move(socket), m_removeCV);
          m_connections.push_back(connection);
          connection->connect();
     }
};