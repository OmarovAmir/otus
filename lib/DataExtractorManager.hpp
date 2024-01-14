#pragma once
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <unordered_map>

#include <DataExtractor.hpp>
#include <SafeQueue.hpp>
#include <fmt/format.h>

/// @brief Менеджер обработчиков команд
class DataExtractorManager
{
    std::size_t _nextHandle;
    std::unordered_map<std::size_t, DataExtractor> _dataExtractorMap;
    std::unordered_map<std::size_t, std::function<void(const std::string&)>> _callBackMap;
    std::shared_mutex _mutex;

    std::vector<std::thread> _logThreads;
    std::mutex _cmdQueueMutex;
    std::shared_ptr<std::condition_variable> _cmdQueueCV;
    CommandBatch::SafeBatchDataQueue _cmdQueue;
    bool finishThreads;

    /// @brief Метод логирования
    void log()
    {
        while (!finishThreads)
        {
            std::vector<CommandBatch::SafeBatchDataQueue::element_type::data_type> data;
            {
                std::unique_lock lock(_cmdQueueMutex);
                _cmdQueueCV->wait(lock, [&] { return finishThreads || !_cmdQueue->empty(); });
                data = _cmdQueue->popAll();
            }
            std::shared_lock lock(_mutex);
            for (const auto& it : data)
            {
                if (auto findResult = _callBackMap.find(it->handle); findResult != _callBackMap.end())
                {
                    std::stringstream stream;
                    for (auto cmd = it->data->cbegin(); cmd != it->data->cend(); ++cmd)
                    {
                        stream << (*cmd)->execute();
                    }
                    auto data = stream.str();
                    findResult->second(data);
                }
            }
        }
    }

  public:
    /// @brief Конструктор
    DataExtractorManager()
        : _cmdQueueCV{std::make_shared<std::condition_variable>()}
        , _cmdQueue{std::make_shared<CommandBatch::SafeBatchDataQueue::element_type>()}
        , finishThreads{false}
    {
        std::size_t hwc = std::thread::hardware_concurrency();
        std::size_t logNumber = (hwc / 2) ? (hwc / 2) : 1;
        _logThreads.reserve(logNumber);
        for (std::size_t i = 0; i < logNumber; ++i)
        {
            _logThreads.emplace_back(&DataExtractorManager::log, this);
        }
    }

    /// @brief Деструктор
    ~DataExtractorManager()
    {
        {
            std::scoped_lock lock(_cmdQueueMutex, _mutex);
            _dataExtractorMap.clear();
        }
        do
        {
            std::scoped_lock lock(_cmdQueueMutex, _mutex);
            if (_cmdQueue->empty())
            {
                finishThreads = true;
            }
            _cmdQueueCV->notify_all();
        }
        while (!finishThreads);
        for (auto& th : _logThreads)
        {
            if (th.joinable())
            {
                th.join();
            }
        }
        {
            std::unique_lock lock(_mutex);
            _callBackMap.clear();
        }
    }

    /// @brief Присоединиться к обработчику команд
    /// @param transmitCallback Функция для отправки ответа клиенту
    /// @return Дескриптор обработчика команд
    std::size_t connect(std::function<void(const std::string&)> transmitCallback)
    {
        std::unique_lock lock(_mutex);
        while (!_dataExtractorMap.try_emplace(_nextHandle, _nextHandle, _cmdQueue, _cmdQueueCV).second)
        {
            ++_nextHandle;
        }
        _callBackMap.emplace(_nextHandle, transmitCallback);
        return _nextHandle++;
    }

    /// @brief Передать команду
    /// @param handle Дескриптор обработчика команд
    /// @param cmd Команда
    void receive(const std::size_t handle, const std::string& cmd)
    {
        std::shared_lock lock(_mutex);
        if (auto findResult = _dataExtractorMap.find(handle); findResult != _dataExtractorMap.end())
        {
            findResult->second.receive(cmd);
        }
    }

    /// @brief Отсоединиться от обработчика команд
    /// @param handle Дескриптор обработчика команд
    void disconnect(const std::size_t handle)
    {
        std::unique_lock lock(_mutex);
        if (auto findResult = _dataExtractorMap.find(handle); findResult != _dataExtractorMap.end())
        {
            _dataExtractorMap.erase(handle);
        }
        if (auto findResult = _callBackMap.find(handle); findResult != _callBackMap.end())
        {
            _callBackMap.erase(handle);
        }
    }
};
