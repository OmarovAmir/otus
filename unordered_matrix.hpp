#ifndef D1145E86_E882_4709_83CF_AD6010409484
#define D1145E86_E882_4709_83CF_AD6010409484

#include <array>
#include <bitset>
#include <cassert>
#include <iostream>
#include <memory>
#include <tuple>
#include <unordered_map>

/**
 * @brief Контейнер представляющий из себя N-мерную матрицу
 *
 * @tparam T тип данных хранимый в матрице
 * @tparam default_value значение по умолчанию
 * @tparam dimension_number число измерений матрицы
 */
template <typename T, T default_value, std::size_t dimension_number = 2>
struct unordered_matrix
{
    /**
     * @brief Тип индекса
     *
     */
    using index_type = std::array<std::size_t, dimension_number>;

    /**
     * @brief Хешируемый тип данных
     *
     */
    using hash_data_type = std::bitset<dimension_number * sizeof(std::size_t) * 8>;

    /**
     * @brief Тип хэш-функции хранилища
     *
     */
    using hash_type = std::hash<hash_data_type>;

    /**
     * @brief Тип ключа хранилища
     *
     */
    using key_type = hash_data_type;

    /**
     * @brief Тип данных хранилища
     *
     */
    using data_type = T;

    /**
     * @brief Тип хранилища данных
     *
     */
    using storage_type = std::unordered_map<key_type, data_type, hash_type>;

    /**
     * @brief Конструктор по умолчанию
     *
     */
    explicit unordered_matrix()
        : _index{std::make_shared<index_type>()}
        , _data{std::make_shared<storage_type>()}
        , _dimension{0}
    {}

    /**
     * @brief Копирующий конструктор с параметрами
     *
     * @param index Указатель на индекс
     * @param data Указатель на данные
     * @param _default_value Значение по умолчанию
     */
    explicit unordered_matrix(const std::shared_ptr<index_type>& index,
                              const std::shared_ptr<storage_type>& data)
        : _index{index}
        , _data{data}
        , _dimension{0}
    {}

    /**
     * @brief Перемещающий конструктор с параметрами
     *
     * @param index Указатель на индекс
     * @param data Указатель на данные
     * @param _default_value Значение по умолчанию
     */
    explicit unordered_matrix(std::shared_ptr<index_type>&& index,
                              std::shared_ptr<storage_type>&& data)
        : _index{std::move(index)}
        , _data{std::move(data)}
        , _dimension{0}
    {}

    /**
     * @brief Копирующий конструктор с параметрами
     *
     * @param index Индекс
     * @param data Данные
     * @param _default_value Значение по умолчанию
     */
    explicit unordered_matrix(const index_type& index,
                              const storage_type& data)
        : unordered_matrix(std::make_shared<index_type>(index),
                           std::make_shared<storage_type>(data))
    {}

    /**
     * @brief Перемещающий конструктор с параметрами
     *
     * @param index Индекс
     * @param data Данные
     * @param _default_value Значение по умолчанию
     */
    explicit unordered_matrix(index_type&& index,
                              storage_type&& data)
        : unordered_matrix(std::make_shared<index_type>(std::move(index)),
                           std::make_shared<storage_type>(std::move(data)))
    {}

    /**
     * @brief Копирующий конструктор
     *
     * @param other Другой экземпляр матрицы
     */
    unordered_matrix(const unordered_matrix<T, default_value, dimension_number>& other)
        : unordered_matrix(*(other._index),
                           *(other._data))
    {}

    /**
     * @brief Перемещающий конструктор
     *
     * @param other Другой экземпляр матрицы
     */
    unordered_matrix(unordered_matrix<T, default_value, dimension_number>&& other)
        : unordered_matrix(std::move(*(other._index)),
                           std::move(*(other._data)))
    {}

    /**
     * @brief Преобразовать индекс в ключ хранилища
     * 
     * @param index Индекс
     * @return Ключ хранилища
     */
    static constexpr key_type index_to_key(const index_type& index)
    {
        key_type key{};
        for (const auto& i : index)
        {
            key <<= bitset_base_data_type;
            key |= i;
        }
        return key;
    }

    /**
     * @brief Преобразовать ключь хранилища в индекс
     * 
     * @param key Ключ хранилища
     * @return Индекс
     */
    static constexpr index_type key_to_index(const key_type& key)
    {
        index_type index{};
        for (std::size_t i = 0; i < dimension_number; ++i)
        {
            index[i] = ((key >> ((dimension_number - i - 1) * bitset_base_data_type)) & mask).to_ulong();
        }
        return index;
    }

    /**
     * @brief Копирующий оператор присванивания
     *
     * @param other Другой экземпляр матрицы
     */
    auto& operator=(const unordered_matrix<T, default_value, dimension_number>& other)
    {
        *(this->_index) = {*(other._index)};
        *(this->_data) = {*(other._data)};
        this->_dimension = 0;
        return *this;
    }

    /**
     * @brief Перемещающий оператор присванивания
     *
     * @param other Другой экземпляр матрицы
     */
    auto& operator=(unordered_matrix<T, default_value, dimension_number>&& other)
    {
        *(this->_index) = std::move(*(other._index));
        *(this->_data) = std::move(*(other._data));
        this->_dimension = 0;
        return *this;
    }

    /**
     * @brief Оператор для установки текущего измерения
     *
     * @param dimension Измерения
     */
    auto& operator()(const std::size_t dimension = 0)
    {
        _dimension = (dimension > _max_dimension) ? _max_dimension : dimension;
        return *this;
    }

    /**
     * @brief Установка значения индекса для текущего измерения
     *
     * @param n Значение индекса для текущего измерения
     */
    auto& operator[](const std::size_t n)
    {
        (*_index)[_dimension] = n;
        if (_dimension != _max_dimension)
        {
            ++(*this);
        }
        else
        {
            -(*this);
        }
        return *this;
    }

    /**
     * @brief Оператор для установки значения по индексу
     *
     * @param value Значение по индексу
     */
    auto& operator=(const data_type& value)
    {
        key_type key = index_to_key(*_index);
        if (value != _default_value)
        {
            (*_data)[key] = value;
        }
        else
        {
            if (auto current_value = _data->find(key); current_value != _data->end())
            {
                _data->erase(current_value);
            }
        }
        -(*this);
        return *this;
    }

    /**
     * @brief Переход к максимальному измерению
     *
     */
    auto& operator+()
    {
        _dimension = _max_dimension;
        return *this;
    }

    /**
     * @brief Переход к минимальному измерению
     *
     */
    auto& operator-()
    {
        _dimension = 0;
        return *this;
    }

    /**
     * @brief Переход к следующему измерению
     *
     */
    auto& operator++()
    {
        if (_dimension != _max_dimension)
        {
            ++_dimension;
        }
        return *this;
    }

    /**
     * @brief Переход к предыдущему измерению
     *
     */
    auto& operator--()
    {
        if (_dimension)
        {
            --_dimension;
        }
        return *this;
    }

    /**
     * @brief Получить значение по текущему индексу
     *
     * @return Значение по текущему индексу
     */
    data_type operator*() const
    {
        key_type key = index_to_key(*_index);
        auto finded = _data->find(key);
        if (finded != _data->end())
        {
            return finded->second;
        }
        return _default_value;
    }

    /**
     * @brief Получить количество хранимых значений
     *
     * @return Количество хранимых значений
     */
    std::size_t size() const
    {
        return _data->size();
    }

    /**
     * @brief Оператор проверки равенства
     *
     * @param value Значение по текущему индексу
     * @return true Значения равны
     * @return false Значения не равны
     */
    bool operator==(const data_type& value) const
    {
        return !(*this != value);
    }

    /**
     * @brief Оператор проверки неравенства
     *
     * @param value Значение по текущему индексу
     * @return true Значения не равны
     * @return false Значения равны
     */
    bool operator!=(const data_type& value) const
    {
        data_type res = _default_value;
        key_type key = index_to_key(*_index);
        auto finded = _data->find(key);
        if (finded != _data->end())
        {
            res = finded->second;
        }
        return (res != value);
    }

    /**
     * @brief Очистить матрицу
     * 
     */
    void clear()
    {
        _data->clear();
    }

    /**
     * @brief Итератор
     *
     * @tparam iterator_type тип базового итератора
     */
    template <typename iterator_type>
    struct iterator
    {
        /**
         * @brief Копирующий конструктор с параметрами
         *
         * @param iterator Значение базового итератора
         */
        explicit iterator(const iterator_type& iterator)
            : _current{iterator}
        {}

        /**
         * @brief Перемещающий конструктор с параметрами
         *
         * @param iterator Значение базового итератора
         */
        explicit iterator(iterator_type&& iterator)
            : _current{std::move(iterator)}
        {}

        /**
         * @brief Префиксный инкремент итератора
         *
         */
        auto& operator++()
        {
            ++_current;
            return *this;
        }

        /**
         * @brief Префиксный декремент итератора
         *
         */
        auto& operator--()
        {
            --_current;
            return *this;
        }

        /**
         * @brief Постфиксный инкремент итератора
         *
         */
        auto operator++(int)
        {
            iterator result{_current++};
            return result;
        }

        /**
         * @brief Постфиксный декремент итератора
         *
         */
        auto operator--(int)
        {
            iterator result{_current--};
            return result;
        }

        /**
         * @brief Разыменовывание итератора
         *
         * @return std::tuple<index[0], ... , index[dimension_number - 1], T value>
         */
        auto operator*() const
        {
            auto& pair = *_current;
            index_type index = key_to_index(pair.first);
            return std::tuple_cat(index, std::tie(pair.second));
        }

        /**
         * @brief Обращению к члену структуры хранилища
         *
         */
        auto operator->() const
        {
            return _current.operator->();
        }

        /**
         * @brief Оператор проверки равенства итераторов
         *
         * @param iterator Итератор
         * @return true Итераторы равны
         * @return false Итераторы не равны
         */
        bool operator==(const iterator& iterator) const
        {
            return !(*this != iterator);
        }

        /**
         * @brief Оператор проверки неравенства итераторов
         *
         * @param iterator Итератор
         * @return true Итераторы не равны
         * @return false Итераторы равны
         */
        bool operator!=(const iterator& iterator) const
        {
            return (_current != iterator._current);
        }

      private:
        /**
         * @brief Базовый итератор
         *
         */
        iterator_type _current;
    };

    /**
     * @brief Получить итератор на начало
     *
     */
    auto begin() const
    {
        return iterator<typename storage_type::iterator>(_data->begin());
    }

    /**
     * @brief Получить итератор на конец
     *
     */
    auto end() const
    {
        return iterator<typename storage_type::iterator>(_data->end());
    }

    /**
     * @brief Получить константный итератор на начало
     *
     */
    auto cbegin() const
    {
        return iterator<typename storage_type::const_iterator>(_data->cbegin());
    }

    /**
     * @brief Получить константный итератор на конец
     *
     */
    auto cend() const
    {
        return iterator<typename storage_type::const_iterator>(_data->cend());
    }

    /**
     * @brief Получить обратный итератор на начало
     *
     */
    auto rbegin() const
    {
        return iterator<typename storage_type::reverse_iterator>(_data->rbegin());
    }

    /**
     * @brief Получить обратный итератор на конец
     *
     */
    auto rend() const
    {
        return iterator<typename storage_type::reverse_iterator>(_data->rend());
    }

    /**
     * @brief Получить константный обратный итератор на начало
     *
     */
    auto crbegin() const
    {
        return iterator<typename storage_type::const_reverse_iterator>(_data->crbegin());
    }

    /**
     * @brief Получить константный обратный итератор на конец
     *
     */
    auto crend() const
    {
        return iterator<typename storage_type::const_reverse_iterator>(_data->crend());
    }

  private:
    /**
     * @brief Значение максимального измерения
     *
     */
    static constexpr std::size_t _max_dimension = dimension_number - 1;

    /**
     * @brief Значение по умолчанию
     *
     */
    static constexpr data_type _default_value = default_value;

    /**
     * @brief Количество бит базового типа данных битовой последовательности
     *
     */
    static constexpr std::size_t bitset_base_data_type = sizeof(std::size_t) * 8;

    /**
     * @brief Битовая маска размера std::size_t
     * 
     */
    static constexpr key_type mask{SIZE_MAX};

    /**
     * @brief Индекс
     *
     */
    std::shared_ptr<index_type> _index;

    /**
     * @brief Хранилище
     *
     */
    std::shared_ptr<storage_type> _data;

    /**
     * @brief Текущее измерение
     *
     */
    std::size_t _dimension;
};

#endif /* D1145E86_E882_4709_83CF_AD6010409484 */
