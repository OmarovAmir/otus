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
    /// @param cmdQueue Очередь потоков логирования
    /// @param cmdQueueCV Условная переменная потоков логирования
    explicit DataExtractor(std::size_t handle, CommandBatch::SafeBatchDataQueue cmdQueue,
                           std::shared_ptr<std::condition_variable> cmdQueueCV)
        : _cmdBatch{std::make_unique<CommandBatch>(handle, std::move(cmdQueue), std::move(cmdQueueCV))}
    {}

    /// @brief Передать команду
    /// @param buffer Указатель на буффер с командой
    /// @param size Размер команды
    void receive(const std::string& cmd)
    {
        if(cmd.empty())
        {
            return;
        }
        auto ICmd = make_command(cmd);
        if (CommandType::LevelUp == ICmd->getType())
        {
            _cmdBatch->levelUp();
            return;
        }
        if (CommandType::LevelDown == ICmd->getType())
        {
            _cmdBatch->levelDown();
            return;
        }
        _cmdBatch->add(ICmd);
    }
};
