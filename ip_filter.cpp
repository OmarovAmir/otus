#include "ip_filter.hpp"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.emplace_back(std::move(str.substr(start, stop - start)));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.emplace_back(std::move(str.substr(start)));

    return r;
}

ip_t split_to_ip_t(std::vector<std::string> vs)
{
    IP ip;
    ip.byte[3] = std::stoi(vs[0]);
    ip.byte[2] = std::stoi(vs[1]);
    ip.byte[1] = std::stoi(vs[2]);
    ip.byte[0] = std::stoi(vs[3]);
    return ip.all;
}

void read_ip_pool(ip_pool_t &ip_pool)
{
    for (std::string line; std::getline(std::cin, line);)
    {
        auto v = std::move(split(line, '\t'));
        ip_pool.emplace_back(std::move(split_to_ip_t(std::move(split(v.at(0), '.')))));
    }
}

void show_ip(const ip_t &ip, std::string end)
{
    IP ip_;
    ip_.all = ip;
    std::cout << unsigned(ip_.byte[3]) << '.' << unsigned(ip_.byte[2]) << '.' << unsigned(ip_.byte[1]) << '.' << unsigned(ip_.byte[0]) << end;
}

void show_ip_pool(const ip_pool_t &ip_pool, std::string end)
{
    for (auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {
        show_ip(*ip, end);
    }
}

void sort_ip_pool(ip_pool_t &ip_pool)
{
    std::sort(ip_pool.begin(), ip_pool.end(), std::greater<ip_t>());
}

ip_pool_t filter(ip_pool_t &ip_pool, std::function<bool(ip_t)> predicate)
{
    ip_pool_t result;
    std::copy_if(ip_pool.begin(), ip_pool.end(),
                 std::back_inserter(result),
                 predicate);
    return result;
}
