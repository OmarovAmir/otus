#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>

template <typename T, T default_value, std::size_t dimension>
struct matrix
{
    std::shared_ptr<std::array<std::size_t, dimension>> _index;
    std::shared_ptr<std::map<std::array<std::size_t, dimension>, T>> _data;
    T _default_value;
    std::size_t _level;

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
        if (_level > 1)
        {
            --_level;
        }
        return *this;
    }

    auto& operator=(const T& value)
    {
        if (_level > 1)
        {
            throw std::length_error("HAHAHA");
        }
        else
        {
            (*_data)[(*_index)] = value;
            return *this;
        }
    }

    T& get()
    {
        if (_level > 1)
        {
            throw std::length_error("HAHAHA");
        }
        else
        {
            if (_data->find((*_index)) != _data->end())
            {
                return (*_data)[(*_index)];
            }
            return _default_value;
        }
    }

    std::size_t size()
    {
        return _data->size();
    }
};

int main()
{
    {
        matrix<int, -1, 3> ml;
        std::cout << "x: " << ml[3][2][1].get() << std::endl;
        std::cout << "size: " << ml.size() << std::endl;
        ml[3][2][1] = 5;
        std::cout << "x: " << ml[3][2][1].get() << std::endl;
        std::cout << "size: " << ml.size() << std::endl;
    }

    return 0;
}
