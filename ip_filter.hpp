#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

std::vector<std::string> split(const std::string &str, char d);
void read_ip_pool(std::vector<std::vector<std::string>> &ip_pool);
void show_ip(const std::vector<std::string> &ip, std::string end = "");
void show_ip_pool(const std::vector<std::vector<std::string>> &ip_pool, std::string end = "");
bool is_ip_greater(const std::vector<std::string> &left, const std::vector<std::string> &right);
void sort_ip_pool(std::vector<std::vector<std::string>> &ip_pool);
std::vector<std::vector<std::string>> filter(std::vector<std::vector<std::string>> &ip_pool, int first);
std::vector<std::vector<std::string>> filter(std::vector<std::vector<std::string>> &ip_pool, int first, int second);
std::vector<std::vector<std::string>> filter_any(std::vector<std::vector<std::string>> &ip_pool, int any);
