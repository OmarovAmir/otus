#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <tuple>

template <typename T, T default_value, std::size_t dimension = 2>
struct matrix
{
    using data_type = T;
    using key_type = std::array<std::size_t, dimension>;
    using storage_type = std::map<key_type, data_type>;

    explicit matrix()
        : _index{std::make_shared<key_type>()}
        , _data{std::make_shared<storage_type>()}
        , _default_value{default_value}
        , _level{0}
    {}

    explicit matrix(const std::shared_ptr<key_type>& index,
                    const std::shared_ptr<storage_type>& data,
                    data_type _default_value = default_value)
        : _index{index}
        , _data{data}
        , _default_value{_default_value}
        , _level{0}
    {}

    explicit matrix(std::shared_ptr<key_type>&& index,
                    std::shared_ptr<storage_type>&& data,
                    data_type _default_value = default_value)
        : _index{std::move(index)}
        , _data{std::move(data)}
        , _default_value{_default_value}
        , _level{0}
    {}

    explicit matrix(const key_type& index,
                    const storage_type& data,
                    data_type _default_value = default_value)
        : matrix(std::make_shared<key_type>(index),
                 std::make_shared<storage_type>(data),
                 _default_value)
    {}

    explicit matrix(key_type&& index,
                    storage_type&& data,
                    data_type _default_value = default_value)
        : matrix(std::make_shared<key_type>(std::move(index)),
                 std::make_shared<storage_type>(std::move(data)),
                 _default_value)
    {}

    matrix(const matrix<T, default_value, dimension>& other)
        : matrix(*(other._index),
                 *(other._data),
                 other._default_value)
    {}

    matrix(matrix<T, default_value, dimension>&& other)
        : matrix(std::move(*(other._index)),
                 std::move(*(other._data)),
                 other._default_value)
    {}

    auto& operator=(const matrix<T, default_value, dimension>& other)
    {
        *(this->_index) = {*(other._index)};
        *(this->_data) = {*(other._data)};
        this->_default_value = other._default_value;
        this->_level = 0;
        return *this;
    }

    auto& operator=(matrix<T, default_value, dimension>&& other)
    {
        *(this->_index) = std::move(*(other._index));
        *(this->_data) = std::move(*(other._data));
        this->_default_value = other._default_value;
        this->_level = 0;
        return *this;
    }

    auto& operator()(const std::size_t level = 0)
    {
        _level = (level > _max_level) ? _max_level : level;
        return *this;
    }

    auto& operator[](const std::size_t n)
    {
        (*_index)[_level] = n;
        if (_level != _max_level)
        {
            ++(*this);
        }
        else
        {
            -(*this);
        }
        return *this;
    }

    auto& operator=(const data_type& value)
    {
        (*_data)[(*_index)] = value;
        -(*this);
        return *this;
    }

    auto& operator+()
    {
        _level = _max_level;
        return *this;
    }

    auto& operator-()
    {
        _level = 0;
        return *this;
    }

    auto& operator++()
    {
        if (_level != _max_level)
        {
            ++_level;
        }
        return *this;
    }

    auto& operator--()
    {
        if (_level)
        {
            --_level;
        }
        return *this;
    }

    data_type operator*() const
    {
        if (_data->find((*_index)) != _data->end())
        {
            return (*_data)[(*_index)];
        }
        return _default_value;
    }

    std::size_t size() const
    {
        return _data->size();
    }

    bool operator==(const data_type& value) const
    {
        return !(*this != value);
    }

    bool operator!=(const data_type& value) const
    {
        data_type res = _default_value;
        if (_data->find((*_index)) != _data->end())
        {
            res = (*_data)[(*_index)];
        }
        return (res != value);
    }

    template <typename iterator_type>
    struct iterator
    {
        explicit iterator(const iterator_type& iterator)
            : _current{iterator}
        {}

        auto& operator++()
        {
            _current++;
            return *this;
        }

        auto& operator--()
        {
            _current--;
            return *this;
        }

        auto& operator++(int)
        {
            ++_current;
            return *this;
        }

        auto& operator--(int)
        {
            --_current;
            return *this;
        }

        auto operator*()
        {
            auto& pair = *_current;
            return std::tuple_cat(pair.first, std::tie(pair.second));
        }

        bool operator==(const iterator& iterator) const
        {
            return !(*this != iterator);
        }

        bool operator!=(const iterator& iterator) const
        {
            return (_current != iterator._current);
        }

      private:
        iterator_type _current;
    };

    auto begin()
    {
        return iterator<typename storage_type::iterator>(_data->begin());
    }

    auto end()
    {
        return iterator<typename storage_type::iterator>(_data->end());
    }

    auto cbegin()
    {
        return iterator<typename storage_type::const_iterator>(_data->cbegin());
    }

    auto cend()
    {
        return iterator<typename storage_type::const_iterator>(_data->cend());
    }

    auto rbegin()
    {
        return iterator<typename storage_type::reverse_iterator>(_data->rbegin());
    }

    auto rend()
    {
        return iterator<typename storage_type::reverse_iterator>(_data->rend());
    }

    auto crbegin()
    {
        return iterator<typename storage_type::const_reverse_iterator>(_data->crbegin());
    }

    auto crend()
    {
        return iterator<typename storage_type::const_reverse_iterator>(_data->crend());
    }

  private:
    static constexpr std::size_t _max_level = dimension - 1;
    std::shared_ptr<key_type> _index;
    std::shared_ptr<storage_type> _data;
    data_type _default_value;
    std::size_t _level;
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

        // выведется одна строка
        // 100100314
        for (const auto c : matrix)
        {
            int x;
            int y;
            int v;
            std::tie(x, y, v) = c;
            std::cout << x << y << v << std::endl;
        }
    }

    {
        // Конструктор по умолчанию
        matrix<int, -1> matrix1;
        assert(matrix1.size() == 0);
        matrix1[0][0] = 0;
        assert(matrix1.size() == 1);

        // Конструктор копирования
        auto matrix2 = matrix1;
        assert(matrix1.size() == 1);
        assert(matrix2.size() == 1);
        matrix1[1][1] = 11;
        assert(matrix1.size() == 2);
        assert(matrix2.size() == 1);

        // Конструктор перемещения
        auto matrix3 = std::move(matrix2);
        assert(matrix2.size() == 0);
        assert(matrix3.size() == 1);
        matrix2[1][1] = 11;
        assert(matrix2.size() == 1);
        assert(matrix3.size() == 1);

        // Конструктор копирования
        auto matrix4{matrix3};
        assert(matrix3.size() == 1);
        assert(matrix4.size() == 1);
        matrix3[1][1] = 11;
        assert(matrix3.size() == 2);
        assert(matrix4.size() == 1);

        // Конструктор перемещения
        auto matrix5{std::move(matrix4)};
        assert(matrix4.size() == 0);
        assert(matrix5.size() == 1);
        matrix4[1][1] = 11;
        assert(matrix4.size() == 1);
        assert(matrix5.size() == 1);

        // Копирующий оператор присваивания
        matrix1[2][2] = 11;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 1);
        matrix5 = matrix1;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 3);

        // Перемещающий оператор присваивания
        matrix1[3][3] = 11;
        assert(matrix1.size() == 4);
        assert(matrix5.size() == 3);
        matrix5 = std::move(matrix1);
        assert(matrix1.size() == 0);
        assert(matrix5.size() == 4);

        // Каноничная форма оператора присваивания
        assert((((matrix1[9][9] = 99) = 88) = 77) == 77);
    }

    return 0;
}
