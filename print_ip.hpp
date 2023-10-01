#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace type_select
{
template <typename T, typename Enable = void>
struct type_select
{
    static void print_ip([[maybe_unused]] const T& v)
    {
        std::cout << "Неопределённый тип" << std::endl;
    }
};

template <typename T>
struct type_select<T, typename std::enable_if<std::is_integral<T>::value>::type>
{
    static void print_ip(const T& v)
    {
        const uint8_t* byte_ptr = reinterpret_cast<const uint8_t*>(&v);
        auto size = sizeof(v);
        for (unsigned int i = 1; i <= sizeof(v); ++i)
        {
            if (i != 1)
            {
                std::cout << ".";
            }
            std::cout << unsigned(*(byte_ptr + (size - (i))));
        }
        std::cout << std::endl;
    }
};

template <typename T>
struct type_select<T, typename std::enable_if<std::is_same<std::string, T>::value>::type>
{
    static void print_ip(const T& v)
    {
        std::cout << v << std::endl;
    }
};

} // namespace type_select

template <typename T>
void print_ip(const T& v)
{
    type_select::type_select<T>::print_ip(v);
};
