#pragma once
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>
#include <vector>

/// @brief Шаблонные класс потокобезопасной очереди
/// @tparam T Тип хранящийся в очереди
template <typename T> class DataQueue
{
    std::queue<T> _data;
    std::mutex _mutex;

  public:
    using data_type = T;

    /// @brief Получить все данные из очереди
    /// @return Массив данных
    std::vector<T> popAll()
    {
        std::unique_lock lock(_mutex);
        std::vector<T> result;
        result.reserve(_data.size());
        while (!_data.empty())
        {
            result.push_back(_data.front());
            _data.pop();
        }
        return result;
    }

    /// @brief Добавить данные в очередь
    /// @param data Данные
    void push(const T& data)
    {
        std::unique_lock lock(_mutex);
        _data.push(data);
    }

    /// @brief Получить размер очереди
    /// @return Размер очереди
    std::size_t size()
    {
        std::unique_lock lock(_mutex);
        return _data.size();
    }

    /// @brief Проверить пуста ли очередь
    /// @return true - очередь пуста
    /// @return false - очередь не пуста
    bool empty()
    {
        std::unique_lock lock(_mutex);
        return _data.empty();
    }
};