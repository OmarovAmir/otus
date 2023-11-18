#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

class Command
{
    std::string _cmd;

  public:
    explicit Command(std::string cmd)
        : _cmd{cmd}
    {}

    std::string execute() const { return _cmd; }
};

class CommandBatch
{
    std::list<Command> _batch;
    std::size_t _size;
    std::size_t _level;
    std::size_t _time;
    std::size_t _logfilenumber;
    inline static const std::string _levelUp{"{"};
    inline static const std::string _levelDow{"}"};

    void save(const std::string& output) const
    {
        std::ofstream logfile("bulk" + std::to_string(_time) + "_" + std::to_string(_logfilenumber) + ".log");
        logfile << output;
        logfile.close();
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
        : _batch{}
        , _size{size}
        , _level{0}
        , _time{0}
        , _logfilenumber{0}
    {}

    ~CommandBatch() { execute(false, true); }

    void add(const std::string cmd)
    {
        bool force = false;
        if (_levelUp == cmd)
        {
            force = !_level;
            ++_level;
        }
        else if (_levelDow == cmd)
        {
            force = !_level;
            if (_level)
            {
                --_level;
            }
        }
        else
        {
            if (!_batch.size())
            {
                std::size_t newTime =
                    std::chrono::seconds(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
                        .count();
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
            _batch.emplace_back(Command{cmd});
        }
        execute(force);
    }
};

class DataExtractor
{
    std::size_t _batchSize;
  public:
    explicit DataExtractor(std::size_t batchSize)
    : _batchSize{batchSize}
    {}

    void run()
    {
        CommandBatch cmdBatch{_batchSize};
        std::string input;
        while (std::getline(std::cin, input))
        {
            if (!input.size())
            {
                continue;
            }
            cmdBatch.add(input);
        }
    }
};

int main()
{
    DataExtractor dxtr{3};
    dxtr.run();
    return 0;
}
