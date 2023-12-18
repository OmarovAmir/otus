#pragma once
#include <list>
#include <mutex>
#include <algorithm>
#include <functional>

template <typename T> class SafeList
{
    std::list<T> _data;
    std::mutex _mutex;

  public:
     std::list<T> readData(std::function<bool(T)> func)
     {
          std::unique_lock lock(_mutex);
          std::list<T> result;

          for(auto it = _data.begin(); it != _data.end();)
          {
               if(func(*it))
               {
                    result.push_back(std::move(*it));
                    it = _data.erase(it);
               }
               else
               {
                    ++it;
               }
          }

          return result;
     }

     void writeData(std::list<T>& data)
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
