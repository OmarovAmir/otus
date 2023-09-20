#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using ip_t = std::vector<std::uint8_t>;
using ip_pool_t = std::vector<ip_t>;

std::vector<std::string> split(const std::string &str, char d);
ip_t split_to_ip_t(std::vector<std::string> vs);
void read_ip_pool(ip_pool_t &ip_pool);
void show_ip(const ip_t &ip, std::string end = "");
void show_ip_pool(const ip_pool_t &ip_pool, std::string end = "");
bool is_ip_greater(const ip_t &left, const ip_t &right);
void sort_ip_pool(ip_pool_t &ip_pool);
ip_pool_t filter(ip_pool_t &ip_pool, std::function<bool(ip_t)> predicate);