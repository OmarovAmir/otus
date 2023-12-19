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
          while (!_data.empty())
          {
               result.push_back(_data.front());
               _data.pop();
          }
          return result;
     }

     void push(const T& data)
     {
          std::unique_lock lock(_mutex);
          _data.push(data);
     }

     std::size_t size()
     {
          std::unique_lock lock(_mutex);
          return _data.size();
     }

     bool empty()
     {
          std::unique_lock lock(_mutex);
          return _data.empty();
     }
};

// Пусть этот список представляет из себя очередь из shared_ptr на list из CommandPtr
// тогда можно создать 2 очереди, где будет храниться копия shared_ptr для логирования и для записи в файл
// тогда сам list из CommandPtr удалится только когда все shared_ptr удалятся, то есть удалится из обеих очередей
