#include "ip_filter.hpp"
#include <gtest/gtest.h>

TEST(ip_filter_tests, greater4)
{
    auto ip1 = split("192.168.127.2", '.');
    auto ip2 = split("192.168.127.1", '.');
    EXPECT_TRUE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, not_greater4)
{
    auto ip1 = split("192.168.127.1", '.');
    auto ip2 = split("192.168.127.2", '.');
    EXPECT_FALSE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, greater3)
{
    auto ip1 = split("192.168.2.127", '.');
    auto ip2 = split("192.168.1.127", '.');
    EXPECT_TRUE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, not_greater3)
{
    auto ip1 = split("192.168.1.127", '.');
    auto ip2 = split("192.168.2.127", '.');
    EXPECT_FALSE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, greater2)
{
    auto ip1 = split("192.2.168.127", '.');
    auto ip2 = split("192.1.168.127", '.');
    EXPECT_TRUE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, not_greater2)
{
    auto ip1 = split("192.1.168.127", '.');
    auto ip2 = split("192.2.168.127", '.');
    EXPECT_FALSE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, greater1)
{
    auto ip1 = split("2.192.168.127", '.');
    auto ip2 = split("1.192.168.127", '.');
    EXPECT_TRUE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, not_greater1)
{
    auto ip1 = split("1.192.168.127", '.');
    auto ip2 = split("2.192.168.127", '.');
    EXPECT_FALSE(is_ip_greater(ip1, ip2));
}

TEST(ip_filter_tests, equal)
{
    auto ip1 = split("192.168.127.1", '.');
    auto ip2 = split("192.168.127.1", '.');
    EXPECT_FALSE(is_ip_greater(ip1, ip2));
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
    std::vector<std::vector<std::string>> ip_pool_before;
    std::vector<std::vector<std::string>> ip_pool_after;
    for (const auto &ip_str : ip_string_pool)
    {
        ip_pool_before.push_back(split(ip_str, '.'));
        ip_pool_after.push_back(split(ip_str, '.'));
    }

    auto is_ip_equal = [](const std::vector<std::string> &left, const std::vector<std::string> &right)
    {
        auto left_ip_part = left.cbegin();
        auto right_ip_part = right.cbegin();
        while (left_ip_part != left.cend())
        {
            auto left_ip_part_int = std::stoi(*left_ip_part);
            auto right_ip_part_int = std::stoi(*right_ip_part);
            if (left_ip_part_int != right_ip_part_int)
            {
                return false;
            }
            ++left_ip_part;
            ++right_ip_part;
        }
        return true;
    };

    auto is_ip_pool_equal = [&is_ip_equal](const std::vector<std::vector<std::string>> &left, const std::vector<std::vector<std::string>> &right)
    {
        auto left_ip = left.cbegin();
        auto right_ip = right.cbegin();
        while (left_ip != left.cend())
        {
            if (!is_ip_equal(*left_ip, *right_ip))
            {
                return false;
            }
            ++left_ip;
            ++right_ip;
        }
        return true;
    };

    for (int i = 0; i < 10; ++i)
    {
        std::random_shuffle(ip_pool_before.begin(), ip_pool_before.end());
        EXPECT_FALSE(is_ip_pool_equal(ip_pool_before, ip_pool_after));
        sort_ip_pool(ip_pool_before);
        EXPECT_TRUE(is_ip_pool_equal(ip_pool_before, ip_pool_after));
    }
}