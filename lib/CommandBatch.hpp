#pragma once

#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <sstream>
#include <iostream>

#include <Command.hpp>
#include <SafeQueue.hpp>

class CommandBatch
{
  public:
    using Data = std::shared_ptr<std::list<CommandPtr>>;
    
    struct BatchData
    {
        std::size_t handle;
        std::size_t time;
        std::size_t number;
        Data data;

        explicit BatchData(std::size_t handle, std::size_t time, std::size_t number, Data data)
        : handle{handle}, time{time}, number{number}, data{data}
        {}
    };

    using SafeBatchDataQueue = std::shared_ptr<SafeQueue<std::shared_ptr<BatchData>>>;

    explicit CommandBatch(std::size_t size, std::size_t handle, 
            SafeBatchDataQueue logData, SafeBatchDataQueue fileSaveData, 
            std::shared_ptr<std::condition_variable> logCV, std::shared_ptr<std::condition_variable> fileSaveCV)
        : _batchPtr{nullptr}
        , _logData{std::move(logData)}
        , _fileSaveData{std::move(fileSaveData)}
        , _logCV{std::move(logCV)}
        , _fileSaveCV{std::move(fileSaveCV)}
        , _size{size}
        , _level{0}
        , _time{0}
        , _logfilenumber{0}
        , _handle{handle}
    {}

    ~CommandBatch()
    {
        std::unique_lock lock(_mutex);
        execute(false, true);
    }

    void levelUp()
    {
        std::unique_lock lock(_mutex);
        if (!_level)
        {
            execute(true);
        }
        ++_level;
    }

    void levelDown()
    {
        std::unique_lock lock(_mutex);
        if (!_level)
        {
            return;
        }
        --_level;
        if (!_level)
        {
            execute(true);
        }
    }

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
    SafeBatchDataQueue _logData;
    SafeBatchDataQueue _fileSaveData;
    std::shared_ptr<std::condition_variable> _logCV;
    std::shared_ptr<std::condition_variable> _fileSaveCV;
    std::size_t _size;
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

    void execute(bool force = false, bool eof = false)
    {
        if (!_batchPtr || !_batchPtr->size())
        {
            return;
        }
        if (!force)
        {
            if (_level)
            {
                return;
            }
            if ((_batchPtr->size() < _size) && !eof)
            {
                return;
            }
        }
        auto data = std::make_shared<BatchData>(_handle, _time, _logfilenumber, _batchPtr);
        _logData->push(data);
        _fileSaveData->push(data);
        _batchPtr.reset();
        _logCV->notify_one();
        _fileSaveCV->notify_one();
    }
};
