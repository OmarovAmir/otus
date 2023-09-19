#include "ip_filter.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    try
    {
        ip_pool_t ip_pool;
        read_ip_pool(ip_pool);
        // Reverse lexicographically sort
        sort_ip_pool(ip_pool);
        show_ip_pool(ip_pool, "\n");

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // Filter by first byte and output
        // ip = filter(1)
        show_ip_pool(filter(ip_pool,
                            [](ip_t ip)
                            {
                                return (ip[0] == "1");
                            }),
                     "\n");

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // Filter by first and second bytes and output
        // ip = filter(46, 70)
        show_ip_pool(filter(ip_pool,
                            [](ip_t ip)
                            {
                                return (ip[0] == "46") &&
                                       (ip[1] == "70");
                            }),
                     "\n");

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // Filter by any byte and output
        // ip = filter_any(46)
        show_ip_pool(filter(ip_pool,
                            [](ip_t ip)
                            {
                                for (auto ip_part : ip)
                                {
                                    if (ip_part == "46")
                                    {
                                        return true;
                                    }
                                }
                                return false;
                            }),
                     "\n");

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}