#pragma once

#include <ICommand.hpp>

/// @brief Основная команда
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

/// @brief Пустая команда
class EmptyCommand final : public ICommand
{
  public:
    explicit EmptyCommand()
        : ICommand(CommandType::Empty)
    {}
};

/// @brief Команда входа в блок с динамическим размером
class LevelUpCommand final : public ICommand
{
  public:
    explicit LevelUpCommand()
        : ICommand(CommandType::LevelUp)
    {}
};

/// @brief Команда выхода из блока с динамическим размером
class LevelDownCommand final : public ICommand
{
  public:
    explicit LevelDownCommand()
        : ICommand(CommandType::LevelDown)
    {}
};

/// @brief Функция создания объекта команды
/// @param cmd Команда
/// @return Указатель на объект команды
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
