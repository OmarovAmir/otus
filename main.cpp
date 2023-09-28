#include "pool_allocator.hpp"
#include <chrono>
#include <iostream>
#include <map>
#include <random>
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
    std::uniform_int_distribution<std::size_t> distrib(2048, 4096);

    auto begin = std::chrono::steady_clock::now();

    for (unsigned long j = 0; j < 1000; ++j)
    {
        auto n = distrib(gen);

        std::vector<int, pool_allocator<int>> v[10];
        // std::vector<int> v[10];
        for (int i = 0; i < 10; ++i)
        {
            for (std::size_t j = 0; j < n; ++j)
            {
                v[i].push_back(j);
            }
            v[i].clear();
        }

        // std::list<int, pool_allocator<int>> l[10];
        // std::list<int> l[10];
        // for (int i = 0; i < 10; ++i)
        // {
        //     for (std::size_t j = 0; j < n; ++j)
        //     {
        //         l[i].push_back(j);
        //     }
        //     l[i].clear();
        // }

        // std::map<int,int, std::less<int>, pool_allocator<std::pair<int,int>>> m[10];
        // std::map<int,int> m[10];
        // for (int i = 0; i < 10; ++i)
        // {
        //     for (std::size_t j = 0; j < n; ++j)
        //     {
        //         m[i][j] = j + 1;
        //     }
        //     m[i].clear();
        // }
    }

    auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time: " << elapsed_ms.count() << " ms\n";

    return 0;
}
