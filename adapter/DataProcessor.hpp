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
     std::shared_ptr<std::condition_variable> m_processedCV;
     std::mutex m_processMutex;
     std::shared_ptr<std::condition_variable> m_processCV;
     bool m_processThreadFinish;
     std::thread m_processThread;

     SafeQueue<DataPtr> m_input_queue;
     SafeQueue<DataPtr> m_output_queue;

     void process()
     {
          while (!m_processThreadFinish)
          {
               std::unique_lock lock(m_processMutex);
               m_processCV->wait(lock, [this] { return m_processThreadFinish || !m_input_queue.empty(); });
               auto in = m_input_queue.popAll();
               for (auto& data: *in)
               {
                    try
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
                         m_output_queue.push(data);
                    }
                    catch(const std::exception& e)
                    {
                         // fmt::println("{}", e.what());
                    }
               }
               if(!m_output_queue.empty())
               {
                    m_processedCV->notify_one();
               }
          }
     }

public:
     explicit DataProcessor(std::shared_ptr<std::condition_variable> processedCV)
     : m_processedCV{processedCV}
     , m_processMutex{}
     , m_processCV{std::make_shared<std::condition_variable>()}
     , m_processThreadFinish{false}
     , m_processThread{&DataProcessor::process, this}
     , m_input_queue{}
     , m_output_queue{}
     {}

     void push(DataPtr& data)
     {
          m_input_queue.push(data);
          m_processCV->notify_one();
     }

     auto popAll()
     {
          auto data = m_output_queue.popAll(); 
          return data;
     }

     bool IsProcessedEmpty()
     {
          return m_output_queue.empty();
     }

     void Clear()
     {
          m_input_queue.clear();
          m_output_queue.clear();
     }

     void Stop()
     {
          {
               std::unique_lock lock(m_processMutex);
               Clear();
               m_processThreadFinish = true;
          }
          m_processCV->notify_one();
          if (m_processThread.joinable())
          {
               m_processThread.join();
          }
     }
};