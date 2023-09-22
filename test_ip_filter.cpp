#include "ip_filter.hpp"
#include <gtest/gtest.h>

namespace
{
[[maybe_unused]] bool is_ip_pool_equal(const ip_pool_t &left, const ip_pool_t &right)
{
    if (left.size() != right.size())
    {
        return false;
    }
    auto left_ip = left.cbegin();
    auto right_ip = right.cbegin();
    while (left_ip != left.cend())
    {
        if (*left_ip != *right_ip)
        {
            return false;
        }
        ++left_ip;
        ++right_ip;
    }
    return true;
}
} // namespace

TEST(ip_filter_tests, greater4)
{
    auto ip1 = split_to_ip_t(split("192.168.127.2", '.'));
    auto ip2 = split_to_ip_t(split("192.168.127.1", '.'));
    EXPECT_TRUE(ip1 > ip2);
}

TEST(ip_filter_tests, not_greater4)
{
    auto ip1 = split_to_ip_t(split("192.168.127.1", '.'));
    auto ip2 = split_to_ip_t(split("192.168.127.2", '.'));
    EXPECT_FALSE(ip1 > ip2);
}

TEST(ip_filter_tests, greater3)
{
    auto ip1 = split_to_ip_t(split("192.168.2.127", '.'));
    auto ip2 = split_to_ip_t(split("192.168.1.127", '.'));
    EXPECT_TRUE(ip1 > ip2);
}

TEST(ip_filter_tests, not_greater3)
{
    auto ip1 = split_to_ip_t(split("192.168.1.127", '.'));
    auto ip2 = split_to_ip_t(split("192.168.2.127", '.'));
    EXPECT_FALSE(ip1 > ip2);
}

TEST(ip_filter_tests, greater2)
{
    auto ip1 = split_to_ip_t(split("192.2.168.127", '.'));
    auto ip2 = split_to_ip_t(split("192.1.168.127", '.'));
    EXPECT_TRUE(ip1 > ip2);
}

TEST(ip_filter_tests, not_greater2)
{
    auto ip1 = split_to_ip_t(split("192.1.168.127", '.'));
    auto ip2 = split_to_ip_t(split("192.2.168.127", '.'));
    EXPECT_FALSE(ip1 > ip2);
}

TEST(ip_filter_tests, greater1)
{
    auto ip1 = split_to_ip_t(split("2.192.168.127", '.'));
    auto ip2 = split_to_ip_t(split("1.192.168.127", '.'));
    EXPECT_TRUE(ip1 > ip2);
}

TEST(ip_filter_tests, not_greater1)
{
    auto ip1 = split_to_ip_t(split("1.192.168.127", '.'));
    auto ip2 = split_to_ip_t(split("2.192.168.127", '.'));
    EXPECT_FALSE(ip1 > ip2);
}

TEST(ip_filter_tests, equal)
{
    auto ip1 = split_to_ip_t(split("192.168.127.1", '.'));
    auto ip2 = split_to_ip_t(split("192.168.127.1", '.'));
    EXPECT_FALSE(ip1 > ip2);
}

TEST(ip_filter_tests, sort)
{
    auto ip_string = "219.102.120.135\t"
                     "185.69.186.168\t"
                     "179.210.145.4\t"
                     "157.39.22.224\t"
                     "113.162.145.156\t"
                     "85.254.10.197\t"
                     "79.180.73.190\t"
                     "67.232.81.208\t"
                     "23.240.215.189\t"
                     "1.29.168.152";

    std::vector<std::string> ip_string_pool = split(ip_string, '\t');
    ip_pool_t ip_pool_before;
    ip_pool_t ip_pool_after;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_before.push_back(split_to_ip_t(split(ip_str, '.')));
        ip_pool_after.push_back(split_to_ip_t(split(ip_str, '.')));
    }

    for (int i = 0; i < 10; ++i)
    {
        std::random_shuffle(ip_pool_before.begin(), ip_pool_before.end());
        EXPECT_FALSE(is_ip_pool_equal(ip_pool_before, ip_pool_after));
        sort_ip_pool(ip_pool_before);
        EXPECT_TRUE(is_ip_pool_equal(ip_pool_before, ip_pool_after));
    }
}

TEST(ip_filter_tests, filter_first)
{
    auto ip_string = "219.102.120.135\t"
                     "185.69.186.168\t"
                     "179.210.145.4\t"
                     "157.39.22.224\t"
                     "113.162.145.156\t"
                     "85.254.10.197\t"
                     "79.180.73.190\t"
                     "67.232.81.208\t"
                     "23.240.215.189\t"
                     "1.29.168.152";

    auto ip_string_after_filter = "1.29.168.152";

    std::vector<std::string> ip_string_pool = split(ip_string, '\t');
    ip_pool_t ip_pool_before;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_before.push_back(split_to_ip_t(split(ip_str, '.')));
    }

    ip_string_pool.clear();

    ip_string_pool = split(ip_string_after_filter, '\t');
    ip_pool_t ip_pool_after;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_after.push_back(split_to_ip_t(split(ip_str, '.')));
    }

    EXPECT_TRUE(is_ip_pool_equal(filter(ip_pool_before,
                                        [](ip_t ip)
                                        {
                                            IP ip_;
                                            ip_.all = ip;
                                            return (ip_.byte[3] == 1);
                                        }),
                                 ip_pool_after));
    EXPECT_FALSE(is_ip_pool_equal(filter(ip_pool_before,
                                         [](ip_t ip)
                                         {
                                             IP ip_;
                                             ip_.all = ip;
                                             return (ip_.byte[3] == 23);
                                         }),
                                  ip_pool_after));
}

TEST(ip_filter_tests, filter_first_second)
{
    auto ip_string = "219.102.120.135\t"
                     "185.69.186.168\t"
                     "179.210.145.4\t"
                     "179.39.22.224\t"
                     "113.162.145.156\t"
                     "85.254.10.197\t"
                     "79.180.73.190\t"
                     "67.232.81.208\t"
                     "23.240.215.189\t"
                     "1.29.168.152";

    auto ip_string_after_filter = "179.39.22.224";

    std::vector<std::string> ip_string_pool = split(ip_string, '\t');
    ip_pool_t ip_pool_before;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_before.push_back(split_to_ip_t(split(ip_str, '.')));
    }

    ip_string_pool.clear();

    ip_string_pool = split(ip_string_after_filter, '\t');
    ip_pool_t ip_pool_after;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_after.push_back(split_to_ip_t(split(ip_str, '.')));
    }

    EXPECT_TRUE(is_ip_pool_equal(filter(ip_pool_before,
                                        [](ip_t ip)
                                        {
                                            IP ip_;
                                            ip_.all = ip;
                                            return (ip_.byte[3] == 179) &&
                                                   (ip_.byte[2] == 39);
                                        }),
                                 ip_pool_after));
    EXPECT_FALSE(is_ip_pool_equal(filter(ip_pool_before,
                                         [](ip_t ip)
                                         {
                                             IP ip_;
                                             ip_.all = ip;
                                             return (ip_.byte[3] == 179) &&
                                                    (ip_.byte[2] == 210);
                                         }),
                                  ip_pool_after));
}

TEST(ip_filter_tests, filter_any)
{
    auto ip_string = "219.102.120.135\t"
                     "185.69.186.168\t"
                     "179.210.145.4\t"
                     "179.39.22.224\t"
                     "113.162.145.156\t"
                     "85.254.10.197\t"
                     "79.180.73.190\t"
                     "67.232.81.208\t"
                     "23.240.215.189\t"
                     "1.29.168.152";

    auto ip_string_after_filter = "185.69.186.168\t"
                                  "1.29.168.152";

    std::vector<std::string> ip_string_pool = split(ip_string, '\t');
    ip_pool_t ip_pool_before;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_before.push_back(split_to_ip_t(split(ip_str, '.')));
    }

    ip_string_pool.clear();

    ip_string_pool = split(ip_string_after_filter, '\t');
    ip_pool_t ip_pool_after;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_after.push_back(split_to_ip_t(split(ip_str, '.')));
    }

    EXPECT_TRUE(is_ip_pool_equal(filter(ip_pool_before,
                                        [](ip_t ip)
                                        {
                                            IP ip_;
                                            ip_.all = ip;
                                            return (ip_.byte[3] == 168) ||
                                                   (ip_.byte[2] == 168) ||
                                                   (ip_.byte[1] == 168) ||
                                                   (ip_.byte[0] == 168);
                                        }),
                                 ip_pool_after));
    EXPECT_FALSE(is_ip_pool_equal(filter(ip_pool_before,
                                         [](ip_t ip)
                                         {
                                             IP ip_;
                                             ip_.all = ip;
                                             return (ip_.byte[3] == 46) ||
                                                    (ip_.byte[2] == 46) ||
                                                    (ip_.byte[1] == 46) ||
                                                    (ip_.byte[0] == 46);
                                         }),
                                  ip_pool_after));
}