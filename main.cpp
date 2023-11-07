#include <array>
#include <cassert>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>

template <typename T, T default_value, std::size_t dimension = 2>
struct matrix
{
    using data_type = T;
    using key_type = std::array<std::size_t, dimension>;
    using storage_type = std::map<key_type, data_type>;

    std::shared_ptr<key_type> _index;
    std::shared_ptr<storage_type> _data;
    data_type _default_value;
    std::size_t _level;
    static constexpr std::size_t _max_level = dimension;
    static constexpr std::size_t _min_level = 1;

    explicit matrix()
        : _index{std::make_shared<key_type>()}
        , _data{std::make_shared<storage_type>()}
        , _default_value{default_value}
        , _level{_max_level}
    {}

    explicit matrix(const std::shared_ptr<key_type>& index,
                    const std::shared_ptr<storage_type>& data,
                    data_type _default_value = default_value)
        : _index{index}
        , _data{data}
        , _default_value{_default_value}
        , _level{_max_level}
    {}

    explicit matrix(std::shared_ptr<key_type>&& index,
                    std::shared_ptr<storage_type>&& data,
                    data_type _default_value = default_value)
        : _index{std::move(index)}
        , _data{std::move(data)}
        , _default_value{_default_value}
        , _level{_max_level}
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
        this->_level = _max_level;
        return *this;
    }

    auto& operator=(matrix<T, default_value, dimension>&& other)
    {
        *(this->_index) = std::move(*(other._index));
        *(this->_data) = std::move(*(other._data));
        this->_default_value = other._default_value;
        this->_level = _max_level;
        return *this;
    }

    auto& operator()(std::size_t level = _max_level)
    {
        _level = (level > _max_level) ? _min_level : level;
        _level = (level < _min_level) ? _max_level : level;
        return *this;
    }

    auto& operator[](const std::size_t n)
    {
        _index->at(_max_level - _level) = n;
        if (_level > _min_level)
        {
            --(*this);
        }
        else
        {
            +(*this);
        }
        return *this;
    }

    auto& operator=(const data_type& value)
    {
        (*_data)[(*_index)] = value;
        +(*this);
        return *this;
    }

    auto& operator+()
    {
        _level = _max_level;
        return *this;
    }

    auto& operator-()
    {
        _level = _max_level;
        return *this;
    }

    auto& operator++()
    {
        ++_level;
        if (_level > _max_level)
        {
            _level = _min_level;
        }
        return *this;
    }

    auto& operator--()
    {
        --_level;
        if (_level < _min_level)
        {
            _level = _max_level;
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
        data_type res = _default_value;
        if (_data->find((*_index)) != _data->end())
        {
            res = (*_data)[(*_index)];
        }
        return (res == value);
    }

    bool operator!=(const data_type& value) const
    {
        return (*this != value);
    }

    void info() const
    {
        std::cout << "_index: { Address: " << _index.get() << "; Data: [ ";
        for (const auto& i : *_index)
        {
            std::cout << i << " ";
        }
        std::cout << "] } _data: { Address: " << _data.get() << "; size: " << _data->size() << " }" << std::endl;
    }

    // std::map<int, int> m;
    // auto im = m.begin();
    // im++;
    // im--;
    // ++im;
    // --im;
    // *im;
    // im->first;

    // struct iterator
    // {
    //     storage_type::iterator
    // }
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
        std::cout << std::endl
                  << "Конструктор по умолчанию" << std::endl;
        matrix<int, -1> matrix1;
        assert(matrix1.size() == 0);
        matrix1[0][0] = 0;
        assert(matrix1.size() == 1);
        matrix1.info();

        std::cout << std::endl
                  << "Конструктор копирования" << std::endl;
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

        std::cout << std::endl
                  << "Конструктор перемещения" << std::endl;
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

        std::cout << std::endl
                  << "Конструктор копирования" << std::endl;
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

        std::cout << std::endl
                  << "Конструктор перемещения" << std::endl;
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

        std::cout << std::endl
                  << "Копирующий оператор присваивания" << std::endl;
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

        std::cout << std::endl
                  << "Перемещающий оператор присваивания" << std::endl;
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

        matrix1[0][0];
        +matrix1[1];
        matrix1.info();
        +matrix1[2];
        matrix1.info();
        +matrix1[3];
        matrix1.info();
        +matrix1[4];
        matrix1.info();

        matrix1()[5];
        matrix1.info();
        matrix1(1)[6];
        matrix1.info();
        matrix1()[7];
        matrix1.info();
        matrix1(1)[8];
        matrix1.info();

        std::cout << std::endl
                  << "Каноничная форма оператора присваивания" << std::endl;
        assert((((matrix1[9][9] = 99) = 88) = 77) == 77);
        matrix1.info();
    }

    return 0;
}
