#pragma once
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <condition_variable>
#include <thread>

#include <DataExtractor.hpp>
#include <SafeQueue.hpp>
#include <FileManager.hpp>

class DataExtractorManager
{
    std::size_t _nextHandle;
    std::unordered_map<std::size_t, DataExtractor> _dataExtractorMap;
    std::shared_mutex _mutex;

    std::thread _logThread;
    std::mutex _logMutex;
    std::shared_ptr<std::condition_variable> _logCV;
    std::thread _fileSaveThread;
    std::mutex _fileSaveMutex;
    std::shared_ptr<std::condition_variable> _fileSaveCV;
    CommandBatch::SafeBatchDataQueue _logData;
    CommandBatch::SafeBatchDataQueue _fileSaveData;
    bool finishThreads;

    void log()
    {
        while(!finishThreads )
        {
            std::unique_lock lock(_logMutex);
            _logCV->wait(lock, [&]{ return finishThreads || !_logData->empty(); });
            auto data = _logData->popAll();

            for (const auto& it: data)
            {
                std::stringstream stream;
                stream << "bulk_mtd_";
                stream << std::to_string(it->handle) << "_";
                stream << std::to_string(it->time) << "_";
                stream << std::to_string(it->number) << ": ";
                for (auto cmd = it->data->cbegin(); cmd != it->data->cend(); ++cmd)
                {
                    if (cmd != it->data->cbegin())
                    {
                        stream << ", ";
                    }
                    stream << (*cmd)->execute();
                }
                stream << std::endl;
                std::cout << stream.str();
            }
        }
    }

    void fileSave()
    {
        while(!finishThreads)
        {
            std::unique_lock lock(_fileSaveMutex);
            _fileSaveCV->wait(lock, [&]{ return finishThreads || !_fileSaveData->empty(); });
            auto data = _fileSaveData->popAll();
            
            for (const auto& it: data)
            {
                std::stringstream filename;
                std::stringstream commands;
                filename << "bulk_mtd_";
                filename << std::to_string(it->handle) << "_";
                filename << std::to_string(it->time) << "_";
                filename << std::to_string(it->number) << ".log";
                for (auto cmd = it->data->cbegin(); cmd != it->data->cend(); ++cmd)
                {
                    if (cmd != it->data->cbegin())
                    {
                        commands << ", ";
                    }
                    commands << (*cmd)->execute();
                }
                commands << std::endl;
                FileManager::save(filename.str(), commands.str());
            }
        }
    }

  public:
    DataExtractorManager()
    : _logCV{std::make_shared<std::condition_variable>()}
    , _fileSaveCV{std::make_shared<std::condition_variable>()}
    , _logData{std::make_shared<CommandBatch::SafeBatchDataQueue::element_type>()}
    , _fileSaveData{std::make_shared<CommandBatch::SafeBatchDataQueue::element_type>()}
    , finishThreads{false}
    {
        _logThread = std::thread(&DataExtractorManager::log, this);
        _fileSaveThread = std::thread(&DataExtractorManager::fileSave, this);
    }

    ~DataExtractorManager()
    {
        std::cout << __FUNCTION__ << std::endl;
        {
            std::scoped_lock lock(_logMutex, _fileSaveMutex);
            finishThreads = true;
        }
        _logCV->notify_one();
        _fileSaveCV->notify_one();
        if(_logThread.joinable())
        {
            _logThread.join();
        }
        if(_fileSaveThread.joinable())
        {
            _fileSaveThread.join();
        }
    }

    std::size_t connect(const std::size_t size)
    {
        std::unique_lock lock(_mutex);
        while (!_dataExtractorMap.try_emplace(_nextHandle, DataExtractor(size, _nextHandle, _logData, _fileSaveData, _logCV, _fileSaveCV)).second)
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
