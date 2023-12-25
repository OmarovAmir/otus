#pragma once

#include <memory>
#include <string>

#include <CommandType.hpp>

/// @brief Интерфейс команды
class ICommand
{
    CommandType _type;

  public:
    /// @brief Конструктор
    /// @param type Тип команды
    explicit ICommand(CommandType type)
        : _type(type)
    {}

    /// @brief Получить тип команды
    /// @return Тип команды
    CommandType getType() const { return _type; }

    /// @brief Вернуть данные команды
    /// @return Данные команды
    virtual std::string execute() const { return std::string(); };
};

using CommandPtr = std::shared_ptr<ICommand>;
