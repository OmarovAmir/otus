#include "print_ip.hpp"


template <typename First, typename... Last>
struct all_same_type
{
    constexpr static bool value = std::is_same_v<std::tuple<First, Last...>, std::tuple<Last..., First>>;
};

template <typename... Types>
struct all_same_type<std::tuple<Types...>> : all_same_type<Types...>
{
};

template <typename... Types>
constexpr bool all_same_type_v = all_same_type<Types...>::value;

int main()
{
    print_ip(int8_t{-1});                           // 255
    print_ip(int16_t{0});                           // 0.0
    print_ip(int32_t{2130706433});                  // 127.0.0.1
    print_ip(int64_t{8875824491850138409});         // 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World!"});         // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400}); // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100}); // 400.300.200.100
    print_ip(std::make_tuple(123, 456, 789, 0));    // 123.456.789.0

    return 0;
}
