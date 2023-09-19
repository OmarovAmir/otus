#include "ip_filter.hpp"

// ("",  '.') -> [""]
// ("11", '.') -> ["11"]
// ("..", '.') -> ["", "", ""]
// ("11.", '.') -> ["11", ""]
// (".11", '.') -> ["", "11"]
// ("11.22", '.') -> ["11", "22"]
ip_t split(const std::string &str, char d)
{
    ip_t r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while (stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void read_ip_pool(ip_pool_t &ip_pool)
{
    for (std::string line; std::getline(std::cin, line);)
    {
        ip_t v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }
}

void show_ip(const ip_t &ip, std::string end)
{
    for (auto ip_part = ip.cbegin();
         ip_part != ip.cend(); ++ip_part)
    {
        if (ip_part != ip.cbegin())
        {
            std::cout << ".";
        }
        std::cout << *ip_part;
    }
    std::cout << end;
}

void show_ip_pool(const ip_pool_t &ip_pool, std::string end)
{
    for (auto ip =
             ip_pool.cbegin();
         ip != ip_pool.cend(); ++ip)
    {
        show_ip(*ip, end);
    }
}

bool is_ip_greater(const ip_t &left, const ip_t &right)
{
    auto left_ip_part = left.cbegin();
    auto right_ip_part = right.cbegin();
    while (left_ip_part != left.cend())
    {
        if (left_ip_part->size() > right_ip_part->size())
        {
            return true;
        }
        if (left_ip_part->size() < right_ip_part->size())
        {
            return false;
        }
        auto left_ip_part_ch = left_ip_part->cbegin();
        auto right_ip_part_ch = right_ip_part->cbegin();
        while (left_ip_part_ch != left_ip_part->cend())
        {
            if (*left_ip_part_ch > *right_ip_part_ch)
            {
                return true;
            }
            if (*left_ip_part_ch < *right_ip_part_ch)
            {
                return false;
            }
            ++left_ip_part_ch;
            ++right_ip_part_ch;
        }
        ++left_ip_part;
        ++right_ip_part;
    }
    return false;
}

void sort_ip_pool(ip_pool_t &ip_pool)
{
    std::sort(ip_pool.begin(), ip_pool.end(),
              [](const ip_t &a, const ip_t &b)
              { return is_ip_greater(a, b); });
}

ip_pool_t filter(ip_pool_t &ip_pool, std::function<bool(ip_t)> predicate)
{
    ip_pool_t result;
    std::copy_if(ip_pool.begin(), ip_pool.end(),
                 std::back_inserter(result),
                 predicate);
    return result;
}
