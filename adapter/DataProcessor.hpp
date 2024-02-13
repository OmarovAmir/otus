#pragma once

#include <Data.hpp>
#include <SafeQueue.hpp>

class DataProcessor
{
     SafeQueue<Data> m_queue;
public:
     void push(Data data)
     {
          m_queue.push(data);
     }
};