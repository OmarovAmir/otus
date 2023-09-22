#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

struct ip_t
{
    uint8_t get_part(const uint8_t index) const
    {
        assert(index < 4);
        IP l_ip;
        l_ip.all = this->ip;
        return l_ip.byte[index];
    }

    void set_part(const uint8_t index, const uint8_t value)
    {
        assert(index < 4);
        IP l_ip;
        l_ip.all = this->ip;
        l_ip.byte[index] = value;
        this->ip = l_ip.all;
    }

    bool operator>(const ip_t &ip) const
    {
        return (this->ip > ip.ip);
    }

    bool operator!=(const ip_t &ip) const
    {
        return (this->ip != ip.ip);
    }

  private:
    union IP
    {
        uint32_t all;
        uint8_t byte[4];
    };

    uint32_t ip;
};

using ip_pool_t = std::vector<ip_t>;

std::vector<std::string> split(const std::string &str, char d);
ip_t split_to_ip_t(std::vector<std::string> vs);
void read_ip_pool(ip_pool_t &ip_pool);
void show_ip(const ip_t &ip, std::string end = "");
void show_ip_pool(const ip_pool_t &ip_pool, std::string end = "");
void sort_ip_pool(ip_pool_t &ip_pool);
ip_pool_t filter(ip_pool_t &ip_pool, std::function<bool(ip_t)> predicate);