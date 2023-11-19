#ifndef BFCB4491_7393_4655_B226_E27BB7278D32
#define BFCB4491_7393_4655_B226_E27BB7278D32

#include <string>
#include <iostream>
#include <list>
#include <chrono>
#include <sstream>
#include <fstream>

#include <Command.hpp>
#include <FileManager.hpp>

class CommandBatch
{
    FileManager _fMngr;
    std::list<Command> _batch;
    std::size_t _size;
    std::size_t _level;
    std::size_t _time;
    std::size_t _logfilenumber;
    bool _forceExecute;
    inline static const std::string _levelUp{"{"};
    inline static const std::string _levelDow{"}"};

    void save(const std::string& output) const
    {
        _fMngr.save("bulk" + std::to_string(_time) + "_" + std::to_string(_logfilenumber) + ".log", output);
    }

    bool commandCheck(const std::string& cmd)
    {
        return (!cmd.empty());
    }

    bool levelCommandCheck(const std::string& cmd)
    {
        _forceExecute = false;
        if (_levelUp == cmd)
        {
            _forceExecute = !_level;
            ++_level;
            return true;
        }
        if (_levelDow == cmd)
        {
            _forceExecute = !_level;
            if (_level)
            {
                --_level;
            }
            return true;
        }
        return false;
    }

    void setTime()
    {
        if (!_batch.size())
        {
            std::size_t newTime =
                std::chrono::seconds(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
                    .count();
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
            stream << cmd->execute();
        }
        stream << std::endl;
        _batch.clear();
        auto output = stream.str();
        std::cout << output;
        save(output);
    }

  public:
    explicit CommandBatch(std::size_t size)
        : _fMngr{}
        , _batch{}
        , _size{size}
        , _level{0}
        , _time{0}
        , _logfilenumber{0}
    {}

    ~CommandBatch() { execute(false, true); }

    void add(const std::string& cmd)
    {
        if(!commandCheck(cmd))
        {
            return;
        }
        if(!levelCommandCheck(cmd))
        {
            setTime();
            _batch.emplace_back(Command{cmd});
        }
        execute(_forceExecute);
    }
};

#endif /* BFCB4491_7393_4655_B226_E27BB7278D32 */
