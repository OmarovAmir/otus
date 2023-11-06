#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>

template <typename T, T default_value, std::size_t dimension = 2>
struct matrix
{
    std::shared_ptr<std::array<std::size_t, dimension>> _index;
    std::shared_ptr<std::map<std::array<std::size_t, dimension>, T>> _data;
    T _default_value;
    std::size_t _level;
    const std::size_t _last_level = 1;

    explicit matrix()
        : _index{std::make_shared<std::array<std::size_t, dimension>>()}
        , _data{std::make_shared<std::map<std::array<std::size_t, dimension>, T>>()}
        , _default_value{default_value}
        , _level{dimension}
    {
    }

    explicit matrix(
        std::shared_ptr<std::array<std::size_t, dimension>>& index,
        std::shared_ptr<std::map<std::array<std::size_t, dimension>, T>>& data,
        T _default_value = default_value,
        std::size_t level = dimension)
        : _index{index}
        , _data{data}
        , _default_value{_default_value}
        , _level{level}
    {
    }

    auto& operator[](std::size_t n)
    {
        _index->at(dimension - _level) = n;
        if (_level > _last_level)
        {
            --_level;
        }
        return *this;
    }

    auto& operator=(const T& value)
    {
        (*_data)[(*_index)] = value;
        return *this;
    }

    T& get()
    {
        if (_data->find((*_index)) != _data->end())
        {
            return (*_data)[(*_index)];
        }
        return _default_value;
    }

    std::size_t size()
    {
        return _data->size();
    }

    std::size_t getLevel()
    {
        return _level;
    }

    void setLevel(std::size_t level)
    {
        _level = level;
    }

    bool operator==(const T& value)
    {
        T res = _default_value;
        if (_data->find((*_index)) != _data->end())
        {
            res = (*_data)[(*_index)];
        }
        return (res == value);
    }
};

int main()
{
    {
        matrix<int, -1> matrix;
        assert(matrix.size() == 0); // все ячейки свободны
        auto a = matrix[0][0];
        assert(a == -1);
        assert(matrix.size() == 0);
        matrix[100][100] = 314;
        assert(matrix[100][100] == 314);
        assert(matrix.size() == 1);

        // // выведется одна строка
        // // 100100314
        // for (auto c : matrix)
        // {
        //     int x;
        //     int y;
        //     int v;
        //     std::tie(x, y, v) = c;
        //     std::cout << x << y << v << std::endl;
        // }
    }

    return 0;
}
