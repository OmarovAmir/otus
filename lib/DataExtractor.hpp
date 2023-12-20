#pragma once

#include <memory>
#include <condition_variable>
#include <cassert>
#include <exception>

#include <CommandBatch.hpp>
#include <SafeQueue.hpp>

/// @brief Обработчик команд
class DataExtractor
{
    std::unique_ptr<CommandBatch> _cmdBatch;

  public:
    
    /// @brief Конструктор
    /// @param batchSize Размер блока команд
    /// @param handle Дескриптор обработчика команд
    /// @param logData Очередь потоков логирования
    /// @param fileSaveData Очередь потоков сохранения в файл
    /// @param logCV Условная переменная потоков логирования
    /// @param fileSaveCV Условная переменная потоков сохранения в файл
    explicit DataExtractor(std::size_t batchSize, std::size_t handle, 
            CommandBatch::SafeBatchDataQueue logData, CommandBatch::SafeBatchDataQueue fileSaveData, 
            std::shared_ptr<std::condition_variable> logCV, std::shared_ptr<std::condition_variable> fileSaveCV)
        : _cmdBatch{std::make_unique<CommandBatch>(batchSize, handle, std::move(logData), std::move(fileSaveData), std::move(logCV), std::move(fileSaveCV))}
    {
        if(0 == batchSize)
        {
            throw std::runtime_error("batchSize can't be equal to zero");
        }
    }

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
