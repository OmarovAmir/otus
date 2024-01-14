#pragma once

#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <sstream>

#include <Command.hpp>
#include <SafeQueue.hpp>

/// @brief Класс блока команд
class CommandBatch
{
  public:
    using Data = std::shared_ptr<std::list<CommandPtr>>;

    /// @brief Данные блока команд
    struct BatchData
    {
        std::size_t handle;
        std::size_t time;
        std::size_t number;
        Data data;

        explicit BatchData(std::size_t handle, std::size_t time, std::size_t number, Data data)
            : handle{handle}
            , time{time}
            , number{number}
            , data{data}
        {}
    };

    using SafeBatchDataQueue = std::shared_ptr<SafeQueue<std::shared_ptr<BatchData>>>;

    /// @brief Конструктор
    /// @param handle Дескриптор обработчика команд
    /// @param cmdQueue Очередь потоков логирования
    /// @param cmdQueueCV Условная переменная потоков логирования
    explicit CommandBatch(std::size_t handle, SafeBatchDataQueue cmdQueue,
                          std::shared_ptr<std::condition_variable> cmdQueueCV)
        : _batchPtr{nullptr}
        , _cmdQueue{std::move(cmdQueue)}
        , _cmdQueueCV{std::move(cmdQueueCV)}
        , _level{0}
        , _time{0}
        , _logfilenumber{0}
        , _handle{handle}
    {}

    /// @brief Деструктор
    ~CommandBatch()
    {
        std::unique_lock lock(_mutex);
        execute();
    }

    /// @brief Вход в блок с динамическим размером
    void levelUp()
    {
        std::unique_lock lock(_mutex);
        execute();
        ++_level;
    }

    /// @brief Выход из блока с динамическим размером
    void levelDown()
    {
        std::unique_lock lock(_mutex);
        if (!_level)
        {
            return;
        }
        --_level;
        execute();
    }

    /// @brief Добавить команду
    /// @param cmd Команда
    void add(const CommandPtr cmd)
    {
        std::unique_lock lock(_mutex);
        checkBatchPtr();
        setTime();
        _batchPtr->emplace_back(cmd);
        execute();
    }

  private:
    Data _batchPtr;
    SafeBatchDataQueue _cmdQueue;
    std::shared_ptr<std::condition_variable> _cmdQueueCV;
    std::size_t _level;
    std::size_t _time;
    std::size_t _logfilenumber;
    std::size_t _handle;
    std::mutex _mutex;

    void checkBatchPtr()
    {
        if (!_batchPtr)
        {
            _batchPtr = std::make_shared<std::list<CommandPtr>>();
        }
    }

    void setTime()
    {
        if (_batchPtr && !_batchPtr->size())
        {
            std::size_t newTime =
                std::chrono::seconds(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())).count();
            if (_time != newTime)
            {
                _logfilenumber = 0;
            }
            else
            {
                ++_logfilenumber;
            }
            _time = newTime;
        }
    }

    void execute()
    {
        if (!_batchPtr || !_batchPtr->size())
        {
            return;
        }
        if (_level)
        {
            return;
        }
        auto data = std::make_shared<BatchData>(_handle, _time, _logfilenumber, _batchPtr);
        _cmdQueue->push(data);
        _batchPtr.reset();
        _cmdQueueCV->notify_one();
    }
};
