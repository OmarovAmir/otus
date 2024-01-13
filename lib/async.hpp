#pragma once
#include <cstddef>
#include <functional>

/// @brief Присоединиться к обработчику команд
/// @return Дескриптор обработчика команд
std::size_t connect(std::function<void(const std::string&)> transmitCallback);

/// @brief Передать команду
/// @param handle Дескриптор обработчика команд
/// @param buffer Указатель на буффер с командой
/// @param size Размер команды
void receive(const std::size_t handle, const std::string& cmd);

/// @brief Отсоединиться от обработчика команд
/// @param handle Дескриптор обработчика команд
void disconnect(const std::size_t handle);
