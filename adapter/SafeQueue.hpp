#pragma once
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>
#include <list>
#include <memory>

/// @brief Шаблонные класс потокобезопасной очереди
/// @tparam T Тип хранящийся в очереди
template <typename T> class SafeQueue
{
    std::unique_ptr<std::list<T>> _data;
    std::mutex _mutex;

    void check()
    {
        if(!_data)
        {
            _data = std::make_unique<std::list<T>>();
        }
    }

  public:
    using data_type = T;

    /// @brief Получить все данные из очереди
    /// @return Массив данных
    std::unique_ptr<std::list<T>> popAll()
    {
        std::unique_lock lock(_mutex);
        check();
        return std::move(_data);
    }

    /// @brief Добавить данные в очередь
    /// @param data Данные
    void push(const T& data)
    {
        std::unique_lock lock(_mutex);
        check();
        _data->push_back(data);
    }

    /// @brief Получить размер очереди
    /// @return Размер очереди
    std::size_t size()
    {
        std::unique_lock lock(_mutex);
        check();
        return _data->size();
    }

    /// @brief Проверить пуста ли очередь
    /// @return true - очередь пуста
    /// @return false - очередь не пуста
    bool empty()
    {
        std::unique_lock lock(_mutex);
        check();
        return _data->empty();
    }

    void clear()
    {
        std::unique_lock lock(_mutex);
        check();
        _data->clear();
    }
};