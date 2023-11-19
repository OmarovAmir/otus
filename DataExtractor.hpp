#ifndef BC6E047B_B766_46E5_9261_E4634670DFDE
#define BC6E047B_B766_46E5_9261_E4634670DFDE

#include <CommandBatch.hpp>

class DataExtractor
{
    std::unique_ptr<CommandBatch> _cmdBatch;

  public:
    explicit DataExtractor(std::size_t batchSize)
        : _cmdBatch{std::make_unique<CommandBatch>(batchSize)}
    {}

    void run()
    {
        std::string input;
        while (std::getline(std::cin, input))
        {
            auto cmd = make_command(input);
            if (CommandType::Empty == cmd->getType())
            {
                continue;
            }
            if (CommandType::LevelUp == cmd->getType())
            {
                _cmdBatch->levelUp();
                continue;
            }
            if (CommandType::LevelDown == cmd->getType())
            {
                _cmdBatch->levelDown();
                continue;
            }
            _cmdBatch->add(cmd);
        }
    }
};

#endif /* BC6E047B_B766_46E5_9261_E4634670DFDE */
