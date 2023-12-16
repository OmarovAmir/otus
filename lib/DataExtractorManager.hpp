#pragma once
#include <DataExtractor.hpp>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>

class DataExtractorManager
{
    std::size_t _nextHandle;
    std::unordered_map<std::size_t, DataExtractor> _dataExtractorMap;
    std::shared_mutex _mutex;

  public:
    std::size_t connect(const std::size_t size)
    {
        std::unique_lock lock(_mutex);
        while (!_dataExtractorMap.try_emplace(_nextHandle, DataExtractor(size, _nextHandle)).second)
        {
            ++_nextHandle;
        }
        return _nextHandle;
    }

    void receive(const std::size_t handle, const void* buffer, const std::size_t size)
    {
        std::shared_lock lock(_mutex);
        if (auto findResult = _dataExtractorMap.find(handle); findResult != _dataExtractorMap.end())
        {
            findResult->second.receive(buffer, size);
        }
    }

    void disconnect(const std::size_t handle)
    {
        std::unique_lock lock(_mutex);
        _dataExtractorMap.erase(handle);
    }
};
