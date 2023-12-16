#pragma once

#include <ICommand.hpp>

class WorkCommand final : public ICommand
{
    std::string _cmd;

  public:
    explicit WorkCommand(std::string cmd)
        : ICommand(CommandType::Work)
        , _cmd{cmd}
    {}

    virtual std::string execute() const override { return _cmd; }
};

class EmptyCommand final : public ICommand
{
  public:
    explicit EmptyCommand()
        : ICommand(CommandType::Empty)
    {}
};

class LevelUpCommand final : public ICommand
{
  public:
    explicit LevelUpCommand()
        : ICommand(CommandType::LevelUp)
    {}
};

class LevelDownCommand final : public ICommand
{
  public:
    explicit LevelDownCommand()
        : ICommand(CommandType::LevelDown)
    {}
};

CommandPtr make_command(const std::string& cmd)
{
    static const std::string _levelUp{"{"};
    static const std::string _levelDown{"}"};

    if (cmd.empty())
    {
        return std::make_shared<EmptyCommand>();
    }
    if (_levelUp == cmd)
    {
        return std::make_shared<LevelUpCommand>();
    }
    if (_levelDown == cmd)
    {
        return std::make_shared<LevelDownCommand>();
    }
    return std::make_shared<WorkCommand>(cmd);
}
