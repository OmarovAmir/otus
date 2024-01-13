#pragma once

#include <cassert>
#include <condition_variable>
#include <exception>
#include <memory>

#include <CommandBatch.hpp>
#include <SafeQueue.hpp>

/// @brief Обработчик команд
class DataExtractor
{
    std::unique_ptr<CommandBatch> _cmdBatch;

  public:
    /// @brief Конструктор
    /// @param handle Дескриптор обработчика команд
    /// @param logData Очередь потоков логирования
    /// @param logCV Условная переменная потоков логирования
    explicit DataExtractor(std::size_t handle, CommandBatch::SafeBatchDataQueue logData,
                           std::shared_ptr<std::condition_variable> logCV)
        : _cmdBatch{std::make_unique<CommandBatch>(handle, std::move(logData), std::move(logCV))}
    {}

    /// @brief Передать команду
    /// @param buffer Указатель на буффер с командой
    /// @param size Размер команды
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
