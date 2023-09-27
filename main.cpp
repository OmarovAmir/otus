#include "pool_allocator.hpp"
#include <chrono>
#include <iostream>
#include <vector>

int main()
{
    auto begin = std::chrono::steady_clock::now();
    // std::vector<int, pool_allocator<int>> v;
    std::vector<int> v;

    for (int i = 0; i < 10000000; ++i)
    {
        v.push_back(i);
    }
    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    return 0;
}
