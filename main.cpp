#include "my_list.hpp"
#include "pool_allocator.hpp"
#include <iostream>
#include <map>
#include <math.h>

int factorial(int n)
{
    int res = 1;
    for (int i = 1; i <= n; i++)
    {
        res = res * i;
    }
    return res;
}

int main()
{
    std::map<int, int> std_a_m;
    std::map<int, int, std::less<int>, pool_allocator<std::pair<int, int>>> custom_a_m;

    for (int i = 0; i < 10; ++i)
    {
        auto fact_i = factorial(i);
        std_a_m.emplace(i, fact_i);
        custom_a_m.emplace(i, fact_i);
    }

    std::cout << "--- map with std::allocator before erase ---" << std::endl;
    for (const auto i : std_a_m)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }

    std::cout << std::endl
              << "--- map with custom allocator before erase ---" << std::endl;
    for (const auto i : custom_a_m)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }

    std_a_m.erase(5);
    custom_a_m.erase(5);

    std::cout << "--- map with std::allocator after erase element with key = 5 ---" << std::endl;
    for (const auto i : std_a_m)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }

    std::cout << std::endl
              << "--- map with custom allocator after erase element with key = 5 ---" << std::endl;
    for (const auto i : custom_a_m)
    {
        std::cout << i.first << " " << i.second << std::endl;
    }

    std_a_m.clear();
    custom_a_m.clear();

    my_list<int> std_mlist;
    my_list<int, pool_allocator<int>> custom_mlist;

    for (int i = 0; i < 10; ++i)
    {
        std_mlist.push_back(i);
        custom_mlist.push_back(i * i);
    }

    std::cout << std::endl
              << "--- my list with std::allocator ---" << std::endl;
    for (const auto& i : std_mlist)
    {
        std::cout << i << std::endl;
    }

    std::cout << std::endl
              << "--- my list with custom allocator ---" << std::endl;
    for (const auto& i : custom_mlist)
    {
        std::cout << i << std::endl;
    }

    std_mlist.clear();
    custom_mlist.clear();

    return 0;
}
