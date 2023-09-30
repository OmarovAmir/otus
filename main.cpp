#include "pool_allocator.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

int main()
{
    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ ((std::mt19937::result_type)
                                                 std::chrono::duration_cast<std::chrono::seconds>(
                                                     std::chrono::system_clock::now().time_since_epoch())
                                                     .count() +
                                             (std::mt19937::result_type)
                                                 std::chrono::duration_cast<std::chrono::microseconds>(
                                                     std::chrono::high_resolution_clock::now().time_since_epoch())
                                                     .count());

    std::mt19937 gen(seed);
    std::uniform_int_distribution<std::size_t> distrib(5, 5);

    auto n = distrib(gen);

    auto begin = std::chrono::steady_clock::now();


    std::vector<int, pool_allocator<int>> v;
    std::list<int, pool_allocator<int>> l;
    std::map<int, int, std::less<int>, pool_allocator<std::pair<int, int>>> m;
    // std::unordered_map<int, int, std::hash<int>, std::equal_to<int>, pool_allocator<std::pair<int, int>>> um;// ???
    
    // std::vector<int> v;
    // std::list<int> l;
    // std::map<int,int> m;
    // std::unordered_map<int,int> um;

    for (std::size_t i = 0; i < n; ++i)
    {
        v.push_back(i);
        l.push_back(i);
        m[i] = i + 1;
        // um[i] = i + 1;
    }

    v.clear();
    l.clear();
    m.clear();
    // um.clear();

    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    return 0;
}
