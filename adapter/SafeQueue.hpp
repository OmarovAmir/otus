#pragma once
#include <algorithm>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>

/// @brief Шаблонные класс потокобезопасной очереди
/// @tparam T Тип хранящийся в очереди
template <typename T> class SafeQueue
{
    std::queue<T> _data;
    std::condition_variable _cv;
    std::atomic_bool _stop;
    std::mutex _mutex;

  public:
    using data_type = T;

    void wait()
    {
        if (!_stop)
        {
            std::unique_lock lock(_mutex);
            _cv.wait(lock, [this] { return _stop || !_data.empty(); });
        }
    }

    void stop()
    {
        _stop = true;
        _cv.notify_one();
    }

    void notify() { _cv.notify_one(); }

    /// @brief Получить данные из очереди
    /// @return Массив данных
    T pop()
    {
        std::unique_lock lock(_mutex);
        T data;
        if (!_data.empty())
        {
            data = _data.front();
            _data.pop();
        }
        return data;
    }

    /// @brief Добавить данные в очередь
    /// @param data Данные
    void push(T&& data)
    {
        if (!_stop)
        {
            std::unique_lock lock(_mutex);
            _data.push(data);
            _cv.notify_one();
        }
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

    /// @brief Очистка очереди
    void clear()
    {
        std::unique_lock lock(_mutex);
        std::queue<T> empty;
        std::swap(_data, empty);
    }
};

template <typename T> using SafeQueuePtr = std::shared_ptr<SafeQueue<T>>;