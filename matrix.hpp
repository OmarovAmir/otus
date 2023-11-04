#include <iostream>
#include <list>
#include <map>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

template <typename T, T default_value, std::size_t matrix_depth>
class matrix
{
  public:

    T operator[](std::size_t n)
    {
        static std::size_t level = 1;
        std::cout << level++ << " - " << n << std::endl;
        return *this;
    }

  private:
    template <typename _T, std::size_t _depth>
    struct matrix_type
    {
        typedef std::map<std::size_t, typename matrix_type<_T, _depth - 1>::type> type;
    };

    template <typename _T>
    struct matrix_type<_T, 1>
    {
        typedef std::map<std::size_t, _T> type;
    };

    template <typename _T, std::size_t _depth>
    using matrix_t = typename matrix_type<_T, _depth>::type;

    matrix_t<T, matrix_depth> _matrix;
    T _default{default_value};
};