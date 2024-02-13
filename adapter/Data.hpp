#pragma once

#include <string>
#include <DataDirection.hpp>

class Data
{
     DataDirection m_direction;
     std::string m_data;
     bool m_processed;

public:
     explicit Data(DataDirection direction, std::string data)
     : m_direction{direction}
     , m_data{data}
     , m_processed{false}
     {}

     bool IsProcessed()
     {
          return m_processed;
     }

     void Processed()
     {
          if(!m_processed)
          {
               switch (m_direction)
               {
               case DataDirection::FromClient:
                    m_direction = DataDirection::ToServer;
                    break;
               
               case DataDirection::FromServer:
                    m_direction = DataDirection::ToClient;
                    break;
               }
               m_processed = true;
          }
     }
};