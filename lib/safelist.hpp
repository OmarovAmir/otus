#pragma once
#include <queue>
#include <vector>
#include <mutex>
#include <algorithm>
#include <functional>

template <typename T> class SafeQueue
{
    std::queue<T> _data;
    std::mutex _mutex;

  public:
     std::vector<T> popAll()
     {
          std::unique_lock lock(_mutex);
          std::vector<T> result;
          

          return result;
     }

     void push()
     {
          std::unique_lock lock(_mutex);
          while(!data.empty())
          {
               _data.push_back(std::move(data.front()));
               data.pop_front();
          }
          // запись данных назад и condition_variable notify_all();
     }
};

// public getData for read or write data
// public getMutex for unique_lock for write and shared_lock for read



// Пусть этот список представляет из себя очередь из shared_ptr на list из CommandPtr
// тогда можно создать 2 очереди, где будет храниться копия shared_ptr для логирования и для записи в файл
// тогда сам list из CommandPtr удалится только когда все shared_ptr удалятся, то есть удалится из обеих очередей
