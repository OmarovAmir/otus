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
        for (unsigned int i = 1; i <= size; ++i)
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

template <typename Test, template <typename...> typename Ref>
struct is_specialization : std::false_type
{
};

template <template <typename...> typename Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type
{
};

template <typename T>
struct type_select<T, typename std::enable_if<is_specialization<T, std::vector>::value || is_specialization<T, std::list>::value>::type>
{
    static void print_ip(const T& v)
    {
        for (auto i = v.cbegin(); i != v.cend(); ++i)
        {
            if (i != v.cbegin())
            {
                std::cout << ".";
            }
            std::cout << *i;
        }
        std::cout << std::endl;
    }
};

template <typename First, typename... T>
struct all_same_type
{
    constexpr static bool value = std::is_same_v<std::tuple<First, T...>, std::tuple<T..., First>>;
};

template <typename... T>
struct all_same_type<std::tuple<T...>> : all_same_type<T...>
{
};

template <typename... T>
constexpr bool all_same_type_v = all_same_type<T...>::value;

template <typename... T>
struct type_select<std::tuple<T...>>
{
    template <typename TupleT, std::size_t... Is>
    static void print_tuple(std::ostream& os, const TupleT& tp, std::index_sequence<Is...>)
    {
        auto printElem = [&os](const auto& x, size_t id)
        {
            if (id > 0)
                os << ".";
            os << x;
        };

        (printElem(std::get<Is>(tp), Is), ...);
        os << std::endl;
    }

    static void print_ip(const std::tuple<T...>& v)
    {
        static_assert(all_same_type_v<std::tuple<T...>>);
        print_tuple(std::cout, v, std::make_index_sequence<std::tuple_size<std::tuple<T...>>::value>{});
    }
};

} // namespace type_select

template <typename T>
void print_ip(const T& v)
{
    type_select::type_select<T>::print_ip(v);
}
