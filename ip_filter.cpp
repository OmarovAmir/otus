#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

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

void show_ip(const std::vector<std::string> &ip, std::string end = "")
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

void show_ip_pool(const std::vector<std::vector<std::string>> &ip_pool)
{
    for (std::vector<std::vector<std::string>>::const_iterator ip =
             ip_pool.cbegin();
         ip != ip_pool.cend(); ++ip)
    {
        show_ip(*ip, "\n");
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

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    try
    {
        std::vector<std::vector<std::string>> ip_pool;
        read_ip_pool(ip_pool);
        sort_ip_pool(ip_pool);
        show_ip_pool(ip_pool);

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)
        filter(ip_pool, 1);

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)
        filter(ip_pool, 46, 70);

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)
        filter_any(ip_pool, 46);

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
