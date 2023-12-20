#pragma once
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <condition_variable>
#include <thread>

#include <DataExtractor.hpp>
#include <SafeQueue.hpp>
#include <FileManager.hpp>

/// @brief Менеджер обработчиков команд
class DataExtractorManager
{
    std::size_t _nextHandle;
    std::unordered_map<std::size_t, DataExtractor> _dataExtractorMap;
    std::shared_mutex _mutex;

    std::vector<std::thread> _logThreads;
    std::mutex _logMutex;
    std::shared_ptr<std::condition_variable> _logCV;
    std::vector<std::thread> _fileSaveThreads;
    std::mutex _fileSaveMutex;
    std::shared_ptr<std::condition_variable> _fileSaveCV;
    CommandBatch::SafeBatchDataQueue _logData;
    CommandBatch::SafeBatchDataQueue _fileSaveData;
    bool finishThreads;

    /// @brief Метод логирования
    void log()
    {
        while(!finishThreads )
        {
            std::vector<CommandBatch::SafeBatchDataQueue::element_type::data_type> data;
            {
                std::unique_lock lock(_logMutex);
                _logCV->wait(lock, [&]{ return finishThreads || !_logData->empty(); });
                data = _logData->popAll();
            }

            for (const auto& it: data)
            {
                std::stringstream stream;
                stream << "bulk_mtd_";
                stream << std::to_string(it->handle) << "_";
                stream << std::to_string(it->time) << "_";
                stream << std::to_string(it->number) << "_";
                stream << std::this_thread::get_id() << ": ";
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

    /// @brief Метод сохранения в файл
    void fileSave()
    {
        while(!finishThreads)
        {
            std::vector<CommandBatch::SafeBatchDataQueue::element_type::data_type> data;
            {
                std::unique_lock lock(_fileSaveMutex);
                _fileSaveCV->wait(lock, [&]{ return finishThreads || !_fileSaveData->empty(); });
                data = _fileSaveData->popAll();
            }
            
            for (const auto& it: data)
            {
                std::stringstream filename;
                std::stringstream commands;
                filename << "bulk_mtd_";
                filename << std::to_string(it->handle) << "_";
                filename << std::to_string(it->time) << "_";
                filename << std::to_string(it->number) << "_";
                filename << std::this_thread::get_id() << ".log";
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

    /// @brief Конструктор
    DataExtractorManager()
    : _logCV{std::make_shared<std::condition_variable>()}
    , _fileSaveCV{std::make_shared<std::condition_variable>()}
    , _logData{std::make_shared<CommandBatch::SafeBatchDataQueue::element_type>()}
    , _fileSaveData{std::make_shared<CommandBatch::SafeBatchDataQueue::element_type>()}
    , finishThreads{false}
    {
        _logThreads.push_back(std::thread(&DataExtractorManager::log, this));
        _logThreads.push_back(std::thread(&DataExtractorManager::log, this));
        _fileSaveThreads.push_back(std::thread(&DataExtractorManager::fileSave, this));
        _fileSaveThreads.push_back(std::thread(&DataExtractorManager::fileSave, this));
        _fileSaveThreads.push_back(std::thread(&DataExtractorManager::fileSave, this));
        _fileSaveThreads.push_back(std::thread(&DataExtractorManager::fileSave, this));
    }

    /// @brief Деструктор
    ~DataExtractorManager()
    {
        {
            std::scoped_lock lock(_logMutex, _fileSaveMutex);
            finishThreads = true;
        }
        _logCV->notify_all();
        _fileSaveCV->notify_all();
        for (auto& th: _logThreads)
        {
            if(th.joinable())
            {
                th.join();
            }
        }
        for (auto& th: _fileSaveThreads)
        {
            if(th.joinable())
            {
                th.join();
            }
        }
    }

    /// @brief Присоединиться к обработчику команд
    /// @param size Размер блока команд
    /// @return Дескриптор обработчика команд
    std::size_t connect(const std::size_t size)
    {
        std::unique_lock lock(_mutex);
        while (!_dataExtractorMap.try_emplace(_nextHandle, size, _nextHandle, _logData, _fileSaveData, _logCV, _fileSaveCV).second)
        {
            ++_nextHandle;
        }
        return _nextHandle++;
    }

    /// @brief Передать команду
    /// @param handle Дескриптор обработчика команд
    /// @param buffer Указатель на буффер с командой
    /// @param size Размер команды
    void receive(const std::size_t handle, const void* buffer, const std::size_t size)
    {
        std::shared_lock lock(_mutex);
        if (auto findResult = _dataExtractorMap.find(handle); findResult != _dataExtractorMap.end())
        {
            findResult->second.receive(buffer, size);
        }
    }

    /// @brief Отсоединиться от обработчика команд
    /// @param handle Дескриптор обработчика команд
    void disconnect(const std::size_t handle)
    {
        std::unique_lock lock(_mutex);
        _dataExtractorMap.erase(handle);
    }
};
