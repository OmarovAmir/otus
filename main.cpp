#include <matrix.hpp>

    template <typename _T, std::size_t _depth>
    struct matrix_type
    {
        typedef std::map<std::size_t, matrix_type<_T, _depth - 1>> type;
        type _m;
        matrix_type<_T, _depth - 1> operator[](std::size_t n)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            std::cout << n << std::endl;
            auto _fm = _m.find(n);
            if (_fm != _m.end())
            {
                std::cout << "Найден" << std::endl;
                return _fm->second;
            }
            std::cout << "Не найден" << std::endl;
            return matrix_type<_T, _depth - 1>{};
        }
    };

    template <typename _T>
    struct matrix_type<_T, 1>
    {
        typedef std::map<std::size_t, _T> type;
        type _m;
        int _default{-1};
        int operator[](std::size_t n)
        {
            std::cout << __PRETTY_FUNCTION__ << std::endl;
            std::cout << n << std::endl;
            auto _fm = _m.find(n);
            if (_fm != _m.end())
            {
                std::cout << "Найден" << std::endl;
                return _fm->second;
            }
            std::cout << "Не найден" << std::endl;
            return _default;
        }
    };

    template <typename _T, std::size_t _depth>
    using matrix_t = typename matrix_type<_T, _depth>::type;

int main()
{
    // matrix<int, -1, 3> m;
    // m[3][1][1];

    matrix_type<int, 5> m;
    m[4][3][2][1][0];

    return 0;
}
