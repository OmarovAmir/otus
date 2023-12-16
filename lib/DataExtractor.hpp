#pragma once

#include <CommandBatch.hpp>

class DataExtractor
{
    std::unique_ptr<CommandBatch> _cmdBatch;

  public:
    explicit DataExtractor(std::size_t batchSize, std::size_t handle)
        : _cmdBatch{std::make_unique<CommandBatch>(batchSize, handle)}
    {}

    void receive(const void* buffer, const std::size_t size)
    {
        std::string input{static_cast<const char*>(buffer), static_cast<const char*>(buffer) + size};
        auto cmd = make_command(input);
        if (CommandType::Empty == cmd->getType())
        {
            return;
        }
        if (CommandType::LevelUp == cmd->getType())
        {
            _cmdBatch->levelUp();
            return;
        }
        if (CommandType::LevelDown == cmd->getType())
        {
            _cmdBatch->levelDown();
            return;
        }
        _cmdBatch->add(cmd);
    }
};
