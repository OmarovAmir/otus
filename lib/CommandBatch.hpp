#pragma once

#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <sstream>

#include <Command.hpp>
#include <FileManager.hpp>

class CommandBatch
{
    std::shared_ptr<std::list<CommandPtr>> _batchPtr;
    std::size_t _size;
    std::size_t _level;
    std::size_t _time;
    std::size_t _logfilenumber;
    std::size_t _handle;
    std::mutex _mutex;

    void save(const std::string& output) const
    {
        FileManager::save("bulk_mtd_" + std::to_string(_handle) + "_" + std::to_string(_time) + "_" +
                              std::to_string(_logfilenumber) + ".log",
                          output);
    }

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
                _logfilenumber = 1;
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
        std::stringstream stream;
        stream << "bulk_mtd_" + std::to_string(_handle) + ": ";
        for (auto cmd = _batchPtr->cbegin(); cmd != _batchPtr->cend(); ++cmd)
        {
            if (cmd != _batchPtr->cbegin())
            {
                stream << ", ";
            }
            stream << (*cmd)->execute();
        }
        stream << std::endl;
        _batchPtr.reset();
        auto output = stream.str();
        std::cout << output;
        save(output);
    }

  public:
    explicit CommandBatch(std::size_t size, std::size_t handle)
        : _batchPtr{nullptr}
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
};
