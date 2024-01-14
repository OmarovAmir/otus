#pragma once

#include <Data.hpp>
#include <ICommand.hpp>
#include <regex>

static Data data;

/// @brief Неизвестная команда
class UnknownCommand final : public ICommand
{
  public:
    explicit UnknownCommand()
        : ICommand(CommandType::Unknown)
    {}

    std::string execute() const
    {
        std::stringstream result;
        result << "Unknown cmd" << std::endl;
        return result.str();
    }
};

/// @brief Команда вставки
class InsertCommand final : public ICommand
{
    std::string _table;
    std::size_t _id;
    std::string _value;

  public:
    explicit InsertCommand(std::string table, std::size_t id, std::string value)
        : ICommand(CommandType::Insert)
        , _table{table}
        , _id{id}
        , _value{value}
    {}

    std::string execute() const { return data.insert(_table, _id, _value); }
};

/// @brief Команда очистки
class TruncateCommand final : public ICommand
{
    std::string _table;

  public:
    explicit TruncateCommand(std::string table)
        : ICommand(CommandType::Truncate)
        , _table{table}
    {}

    std::string execute() const { return data.truncate(_table); }
};

/// @brief Команда поиска пересечений
class IntersectionCommand final : public ICommand
{
  public:
    explicit IntersectionCommand()
        : ICommand(CommandType::Intersection)
    {}

    std::string execute() const { return data.intersection(); }
};

/// @brief Команда поиска различий
class SymmetricDifferenceCommand final : public ICommand
{
  public:
    explicit SymmetricDifferenceCommand()
        : ICommand(CommandType::SymmetricDifference)
    {}

    std::string execute() const { return data.symmetric_difference(); }
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
    if (_levelUp == cmd)
    {
        return std::make_shared<LevelUpCommand>();
    }
    if (_levelDown == cmd)
    {
        return std::make_shared<LevelDownCommand>();
    }
    std::smatch sm;
    if (std::regex_match(cmd, sm, std::regex{"^INSERT ([A,B]) ([0-9]+) (\\S+)$"}))
    {
        std::stringstream stream(sm[2].str());
        size_t id;
        stream >> id;
        return std::make_shared<InsertCommand>(sm[1].str(), id, sm[3].str());
    }
    if (std::regex_match(cmd, sm, std::regex{"^TRUNCATE ([A,B])$"}))
    {
        return std::make_shared<TruncateCommand>(sm[1].str());
    }
    if (std::regex_match(cmd, sm, std::regex{"^INTERSECTION$"}))
    {
        return std::make_shared<IntersectionCommand>();
    }
    if (std::regex_match(cmd, sm, std::regex{"^SYMMETRIC_DIFFERENCE$"}))
    {
        return std::make_shared<SymmetricDifferenceCommand>();
    }
    return std::make_shared<UnknownCommand>();
}
