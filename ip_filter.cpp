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
        r.push_back(str.substr(start, stop - start));

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}

void read_ip_pool(std::vector<std::vector<std::string>> &ip_pool)
{
    for (std::string line; std::getline(std::cin, line);)
    {
        std::vector<std::string> v = split(line, '\t');
        ip_pool.push_back(split(v.at(0), '.'));
    }
}

void show_ip(const std::vector<std::string> &ip, std::string end)
{
    for (std::vector<std::string>::const_iterator ip_part = ip.cbegin();
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

void show_ip_pool(const std::vector<std::vector<std::string>> &ip_pool, std::string end)
{
    for (std::vector<std::vector<std::string>>::const_iterator ip =
             ip_pool.cbegin();
         ip != ip_pool.cend(); ++ip)
    {
        show_ip(*ip, end);
    }
}

bool is_ip_greater(const std::vector<std::string> &left, const std::vector<std::string> &right)
{
    std::vector<std::string>::const_iterator left_ip_part = left.cbegin();
    std::vector<std::string>::const_iterator right_ip_part = right.cbegin();
    while (left_ip_part != left.cend())
    {
        auto left_ip_part_int = std::stoi(*left_ip_part);
        auto right_ip_part_int = std::stoi(*right_ip_part);
        if (left_ip_part_int > right_ip_part_int)
        {
            return true;
        }
        if (left_ip_part_int < right_ip_part_int)
        {
            return false;
        }
        ++left_ip_part;
        ++right_ip_part;
    }
    return false;
}

void sort_ip_pool(std::vector<std::vector<std::string>> &ip_pool)
{
    std::sort(ip_pool.begin(), ip_pool.end(),
              [](const std::vector<std::string> &a, const std::vector<std::string> &b)
              { return is_ip_greater(a, b); });
}

void filter(std::vector<std::vector<std::string>> &ip_pool, int first)
{
    std::for_each(ip_pool.begin(), ip_pool.end(),
                  [&first](std::vector<std::string> ip)
                  {
                      std::vector<std::string>::const_iterator ip_part = ip.cbegin();
                      if (std::stoi(*ip_part) == first)
                      {
                          show_ip(ip, "\n");
                      }
                  });
}

void filter(std::vector<std::vector<std::string>> &ip_pool, int first, int second)
{
    std::for_each(ip_pool.begin(), ip_pool.end(),
                  [&first, &second](std::vector<std::string> ip)
                  {
                      std::vector<std::string>::const_iterator ip_part = ip.cbegin();

                      if (std::stoi(*ip_part) == first)
                      {
                          ++ip_part;
                          if (std::stoi(*ip_part) == second)
                          {
                              show_ip(ip, "\n");
                          }
                      }
                  });
}

void filter_any(std::vector<std::vector<std::string>> &ip_pool, int any)
{
    std::for_each(ip_pool.begin(), ip_pool.end(),
                  [&any](std::vector<std::string> ip)
                  {
                      std::vector<std::string>::const_iterator ip_part = ip.cbegin();
                      while (ip_part != ip.cend())
                      {
                          if (std::stoi(*ip_part) == any)
                          {
                              show_ip(ip, "\n");
                              break;
                          }
                          ++ip_part;
                      }
                  });
}
