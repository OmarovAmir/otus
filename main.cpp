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
    static constexpr std::size_t _last_level = 1;

    explicit matrix()
        : _index{std::make_shared<std::array<std::size_t, dimension>>()}
        , _data{std::make_shared<std::map<std::array<std::size_t, dimension>, T>>()}
        , _default_value{default_value}
        , _level{dimension}
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        const std::shared_ptr<std::array<std::size_t, dimension>>& index,
        const std::shared_ptr<std::map<std::array<std::size_t, dimension>, T>>& data,
        T _default_value = default_value,
        std::size_t level = dimension)
        : _index{index}
        , _data{data}
        , _default_value{_default_value}
        , _level{level}
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        std::shared_ptr<std::array<std::size_t, dimension>>&& index,
        std::shared_ptr<std::map<std::array<std::size_t, dimension>, T>>&& data,
        T _default_value = default_value,
        std::size_t level = dimension)
        : _index{std::move(index)}
        , _data{std::move(data)}
        , _default_value{_default_value}
        , _level{level}
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        const std::array<std::size_t, dimension>& index,
        const std::map<std::array<std::size_t, dimension>, T>& data,
        T _default_value = default_value,
        std::size_t level = dimension)
        : matrix(std::make_shared<std::array<std::size_t, dimension>>(index),
                 std::make_shared<std::map<std::array<std::size_t, dimension>, T>>(data),
                 _default_value, 
                 level)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        std::array<std::size_t, dimension>&& index,
        std::map<std::array<std::size_t, dimension>, T>&& data,
        T _default_value = default_value,
        std::size_t level = dimension)
        : matrix(std::make_shared<std::array<std::size_t, dimension>>(std::move(index)),
                 std::make_shared<std::map<std::array<std::size_t, dimension>, T>>(std::move(data)),
                 _default_value, 
                 level)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    matrix(
        const matrix<T, default_value, dimension>& other)
        : matrix(*(other._index),
                 *(other._data),
                 other._default_value, 
                 other._level)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    matrix(
        matrix<T, default_value, dimension>&& other)
        : matrix(std::move(*(other._index)),
                 std::move(*(other._data)),
                 other._default_value, 
                 other._level)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    matrix<T, default_value, dimension>& operator=(
        const matrix<T, default_value, dimension>& other)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        this->_index = {*(other._index)};
        this->_data = {*(other._data)};
        this->_default_value = other._default_value;
        this->_level = _level;
        return *this;
    }

    matrix<T, default_value, dimension>& operator=(
        matrix<T, default_value, dimension>&& other)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        *(this->_index) = std::move(*(other._index));
        *(this->_data) = std::move(*(other._data));
        this->_default_value = other._default_value;
        this->_level = _level;
        return *this;
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

    T get()
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
        // matrix<int, -1> matrix;
        // assert(matrix.size() == 0); // все ячейки свободны

        // auto a = matrix[0][0];
        // assert(a == -1);
        // assert(matrix.size() == 0);

        // matrix[100][100] = 314;
        // assert(matrix[100][100] == 314);
        // assert(matrix.size() == 1);      

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

    {
        matrix<int, -1> matrix;
        assert(matrix.size() == 0);
        matrix[0][0] = 10;
        matrix[0][1] = 20;
        assert(matrix.size() == 2);
        std::cout << std::endl << std::endl;
        auto x = matrix;
        assert(x.size() == 2);
        assert(matrix.size() == 2);
        std::cout << std::endl << std::endl;
        auto b = std::move(matrix);
        assert(b.size() == 2);
        assert(matrix.size() == 0);
        std::cout << std::endl << std::endl;
        auto k{b};
        assert(b.size() == 2);
        assert(k.size() == 2);
        std::cout << std::endl << std::endl;
        auto l{std::move(b)};
        assert(b.size() == 0);
        assert(k.size() == 2);

    }


    return 0;
}
