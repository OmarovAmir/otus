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
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        const std::shared_ptr<std::array<std::size_t, dimension>>& index,
        const std::shared_ptr<std::map<std::array<std::size_t, dimension>, T>>& data,
        T _default_value = default_value)
        : _index{index}
        , _data{data}
        , _default_value{_default_value}
        , _level{dimension}
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        std::shared_ptr<std::array<std::size_t, dimension>>&& index,
        std::shared_ptr<std::map<std::array<std::size_t, dimension>, T>>&& data,
        T _default_value = default_value)
        : _index{std::move(index)}
        , _data{std::move(data)}
        , _default_value{_default_value}
        , _level{dimension}
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        const std::array<std::size_t, dimension>& index,
        const std::map<std::array<std::size_t, dimension>, T>& data,
        T _default_value = default_value)
        : matrix(std::make_shared<std::array<std::size_t, dimension>>(index),
                 std::make_shared<std::map<std::array<std::size_t, dimension>, T>>(data),
                 _default_value)
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    explicit matrix(
        std::array<std::size_t, dimension>&& index,
        std::map<std::array<std::size_t, dimension>, T>&& data,
        T _default_value = default_value)
        : matrix(std::make_shared<std::array<std::size_t, dimension>>(std::move(index)),
                 std::make_shared<std::map<std::array<std::size_t, dimension>, T>>(std::move(data)),
                 _default_value)
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    matrix(
        const matrix<T, default_value, dimension>& other)
        : matrix(*(other._index),
                 *(other._data),
                 other._default_value)
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    matrix(
        matrix<T, default_value, dimension>&& other)
        : matrix(std::move(*(other._index)),
                 std::move(*(other._data)),
                 other._default_value)
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    matrix<T, default_value, dimension>& operator=(
        const matrix<T, default_value, dimension>& other)
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
        *(this->_index) = {*(other._index)};
        *(this->_data) = {*(other._data)};
        this->_default_value = other._default_value;
        this->_level = dimension;
        return *this;
    }

    matrix<T, default_value, dimension>& operator=(
        matrix<T, default_value, dimension>&& other)
    {
        // std::cout << __PRETTY_FUNCTION__ << std::endl;
        *(this->_index) = std::move(*(other._index));
        *(this->_data) = std::move(*(other._data));
        this->_default_value = other._default_value;
        this->_level = dimension;
        return *this;
    }

    matrix<T, default_value, dimension>& operator[](std::size_t n)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "level: " << _level << std::endl;
        std::cout << "n: " << n << std::endl;
        std::cout << "dimension: " << dimension << std::endl;
        _index->at(dimension - _level) = n;

        for(const auto& i: *_index)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;

        if (_level > _last_level)
        {
            --_level;
        }
        else
        {
            _level = dimension;
        }
        return *this;
    }

    matrix<T, default_value, dimension>& operator=(const T& value)
    {
        (*_data)[(*_index)] = value;
        _level = dimension;
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

    void resetLevel(std::size_t level)
    {
        _level = dimension;
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

    void info()
    {
        std::cout << "_index: { Address: " << _index.get() << "; Data: [ ";
        for(const auto& i: *_index)
        {
            std::cout << i << " ";
        }
        std::cout << "] } _data: { Address: " << _data.get() << "; size: " << _data->size() << " }" << std::endl;
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
        //     int matrix2;
        //     int y;
        //     int v;
        //     std::tie(matrix2, y, v) = c;
        //     std::cout << matrix2 << y << v << std::endl;
        // }
    }

    {
        std::cout << std::endl << "Конструктор по умолчанию" << std::endl;
        matrix<int, -1> matrix1;
        assert(matrix1.size() == 0);
        matrix1[0][0] = 0;
        assert(matrix1.size() == 1);
        matrix1.info();

        std::cout << std::endl << "Конструктор копирования" << std::endl;
        auto matrix2 = matrix1;
        assert(matrix1.size() == 1);
        assert(matrix2.size() == 1);
        matrix1.info();
        matrix2.info();
        matrix1[1][1] = 11;
        assert(matrix1.size() == 2);
        assert(matrix2.size() == 1);
        matrix1.info();
        matrix2.info();

        std::cout << std::endl << "Конструктор перемещения" << std::endl;
        auto matrix3 = std::move(matrix2);
        assert(matrix2.size() == 0);
        assert(matrix3.size() == 1);
        matrix2.info();
        matrix3.info();
        matrix2[1][1] = 11;
        assert(matrix2.size() == 1);
        assert(matrix3.size() == 1);
        matrix2.info();
        matrix3.info();

        std::cout << std::endl << "Конструктор копирования" << std::endl;
        auto matrix4{matrix3};
        assert(matrix3.size() == 1);
        assert(matrix4.size() == 1);
        matrix3.info();
        matrix4.info();
        matrix3[1][1] = 11;
        assert(matrix3.size() == 2);
        assert(matrix4.size() == 1);
        matrix3.info();
        matrix4.info();

        std::cout << std::endl << "Конструктор перемещения" << std::endl;
        auto matrix5{std::move(matrix4)};
        assert(matrix4.size() == 0);
        assert(matrix5.size() == 1);
        matrix4.info();
        matrix5.info();
        matrix4[1][1] = 11;
        assert(matrix4.size() == 1);
        assert(matrix5.size() == 1);
        matrix4.info();
        matrix5.info();

        std::cout << std::endl << "Копирующий оператор присваивания" << std::endl;
        matrix1[2][2] = 11;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 1);
        matrix1.info();
        matrix5.info();
        matrix5 = matrix1;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 3);
        matrix1.info();
        matrix5.info();

        std::cout << std::endl << "Перемещающий оператор присваивания" << std::endl;
        matrix1[3][3] = 11;
        assert(matrix1.size() == 4);
        assert(matrix5.size() == 3);
        matrix1.info();
        matrix5.info();
        matrix5 = std::move(matrix1);
        assert(matrix1.size() == 0);
        assert(matrix5.size() == 4);
        matrix1.info();
        matrix5.info();

        std::cout << std::endl << "Каноничная форма оператора присваивания" << std::endl;
        assert((((matrix1[9][9] = 99 ) = 88 ) = 77) == 77);
        matrix1.info();
    }


    return 0;
}
