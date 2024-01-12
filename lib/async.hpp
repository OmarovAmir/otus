#pragma once
#include <cstddef>

/// @brief Присоединиться к обработчику команд
/// @param size Размер блока команд
/// @return Дескриптор обработчика команд
std::size_t connect(const std::size_t size);

/// @brief Передать команду
/// @param handle Дескриптор обработчика команд
/// @param buffer Указатель на буффер с командой
/// @param size Размер команды
void receive(const std::size_t handle, const void* buffer, const std::size_t size);

/// @brief Отсоединиться от обработчика команд
/// @param handle Дескриптор обработчика команд
void disconnect(const std::size_t handle);
