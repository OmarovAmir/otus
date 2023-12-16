#pragma once
#include <cstddef>

std::size_t connect(const std::size_t size);
void receive(const std::size_t handle, const void* buffer, const std::size_t size);
void disconnect(const std::size_t handle);
