#pragma once
#include <DataExtractor.hpp>
#include <unordered_map>

class DataExtractorManager
{
    std::size_t nextHandle;
    std::unordered_map<std::size_t, DataExtractor> _dataExtractorMap;

  public:
    std::size_t connect(const std::size_t size)
    {
        // mutex for nextHandle and _dataExtractorMap
        while (!_dataExtractorMap.try_emplace(nextHandle, DataExtractor(size, nextHandle)).second)
        {
            ++nextHandle;
        }
        return nextHandle;
    }

    void receive(const std::size_t handle, const void* buffer, const std::size_t size)
    {
        // mutex for _dataExtractorMap
        if (auto findResult = _dataExtractorMap.find(handle); findResult != _dataExtractorMap.end())
        {
            findResult->second.receive(buffer, size);
        }
    }

    void disconnect(const std::size_t handle)
    {
        // mutex for _dataExtractorMap
        _dataExtractorMap.erase(handle);
    }
};
