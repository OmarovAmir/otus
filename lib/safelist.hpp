#pragma once
#include <list>
#include <mutex>
#include <shared_mutex>

template <typename T> class SafeList
{
    std::list<T> _data;
    std::shared_mutex _mutex;

  public:
    
};

// public getData for read or write data
// public getMutex for unique_lock for write and shared_lock for read
