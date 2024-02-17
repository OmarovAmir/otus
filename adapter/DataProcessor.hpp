#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>

#include <Data.hpp>
#include <SafeQueue.hpp>
#include <Base64.hpp>

class DataProcessor
{
     SafeQueuePtr<DataPtr> m_input_queue;
     SafeQueuePtr<DataPtr> m_output_queue;
     std::atomic_bool m_processThreadFinish;
     std::thread m_processThread;

     void process()
     {
          fmt::println("{} start", __FUNCTION__);
          while (!m_processThreadFinish)
          {
               m_input_queue->wait();
               auto data = m_input_queue->pop();
               fmt::println("{} execute", __FUNCTION__);
               try
               {
                    if(data)
                    {
                         switch(data->GetDirection())
                         {
                              case DataDirection::FromClient:
                                   data->SetData(base64::to_base64(data->GetData()));
                                   break;
                              case DataDirection::FromServer:
                                   data->SetData(base64::from_base64(data->GetData()));
                                   break;
                              default:
                                   break;
                         }
                         data->Processed();
                         m_output_queue->push(std::move(data));
                    }
               }
               catch(const std::exception& e)
               {
                    // fmt::println("{}", e.what());
               }
          }
          fmt::println("{} end", __FUNCTION__);
     }

public:
     explicit DataProcessor(SafeQueuePtr<DataPtr> input_queue, SafeQueuePtr<DataPtr> output_queue)
     : m_input_queue{input_queue}
     , m_output_queue{output_queue}
     , m_processThreadFinish{false}
     , m_processThread{&DataProcessor::process, this}
     {
          m_processThread.detach();
     }
     ~DataProcessor()
     {
          fmt::println("{} start", __FUNCTION__);
          m_processThreadFinish = true;
          m_input_queue->clear();
          m_input_queue->stop();
          fmt::println("{} end", __FUNCTION__);
     }
};