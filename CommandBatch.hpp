#ifndef BFCB4491_7393_4655_B226_E27BB7278D32
#define BFCB4491_7393_4655_B226_E27BB7278D32

#include <chrono>
#include <list>
#include <sstream>

#include <Command.hpp>
#include <FileManager.hpp>

class CommandBatch
{
    std::list<CommandPtr> _batch;
    std::size_t _size;
    std::size_t _level;
    std::size_t _time;
    std::size_t _logfilenumber;

    void save(const std::string& output) const
    {
        FileManager::save("bulk" + std::to_string(_time) + "_" + std::to_string(_logfilenumber) + ".log", output);
    }

    void setTime()
    {
        if (!_batch.size())
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
        if (!_batch.size())
        {
            return;
        }
        if (!force)
        {
            if (_level)
            {
                return;
            }
            if ((_batch.size() < _size) && !eof)
            {
                return;
            }
        }
        std::stringstream stream;
        stream << "bulk: ";
        for (auto cmd = _batch.cbegin(); cmd != _batch.cend(); ++cmd)
        {
            if (cmd != _batch.cbegin())
            {
                stream << ", ";
            }
            stream << (*cmd)->execute();
        }
        stream << std::endl;
        _batch.clear();
        auto output = stream.str();
        std::cout << output;
        save(output);
    }

  public:
    explicit CommandBatch(std::size_t size)
        : _batch{}
        , _size{size}
        , _level{0}
        , _time{0}
        , _logfilenumber{0}
    {}

    ~CommandBatch() { execute(false, true); }

    void levelUp()
    {
        if (!_level)
        {
            execute(true);
        }
        ++_level;
    }

    void levelDown()
    {
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
        setTime();
        _batch.emplace_back(cmd);
        execute();
    }
};

#endif /* BFCB4491_7393_4655_B226_E27BB7278D32 */
