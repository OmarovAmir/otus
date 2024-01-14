#pragma once
#include <array>
#include <fmt/format.h>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>

class Data
{
    struct Item
    {
        std::size_t _id;
        std::string _value;
        explicit Item(std::size_t id, std::string value)
            : _id{id}
            , _value{value}
        {}

        bool operator<(const Item& other) const
        {
            if (_id < other._id)
                return true;
            return false;
        };
    };

    std::array<std::set<Item>, 2> _tables;
    std::mutex _mtx;

  public:
    std::string insert(std::string table, std::size_t id, std::string value)
    {
        std::stringstream result;
        std::unique_lock lock(_mtx);
        Item item{id, value};
        auto& tbl = _tables.at((table == "A") ? 0 : 1);
        if (tbl.find(item) != tbl.end())
        {
            result << "ERR dublicate " << id << std::endl;
        }
        else
        {
            if (tbl.emplace(item).second)
            {
                result << "OK" << std::endl;
            }
            else
            {
                result << "ERR while inserting" << std::endl;
            }
        }
        for (const auto& item : tbl)
        {
            fmt::println("{} {}", item._id, item._value);
        }
        return result.str();
    }

    std::string truncate(std::string table)
    {
        std::stringstream result;
        std::unique_lock lock(_mtx);
        auto& tbl = _tables.at((table == "A") ? 0 : 1);
        tbl.clear();
        result << "OK" << std::endl;
        return result.str();
    }

    std::string symmetric_difference()
    {
        std::stringstream result;
        std::unique_lock lock(_mtx);
        auto first1 = _tables[0].begin();
        auto last1 = _tables[0].end();
        auto first2 = _tables[1].begin();
        auto last2 = _tables[1].end();
        while (first1 != last1)
        {
            if (first2 == last2)
            {
                result << fmt::format("{},{},", first1->_id, first1->_value) << std::endl;
                ++first1;
                continue;
            }

            if (*first1 < *first2)
            {
                result << fmt::format("{},{},", first1->_id, first1->_value) << std::endl;
                ++first1;
            }
            else
            {
                if (*first2 < *first1)
                {
                    result << fmt::format("{},,{}", first2->_id, first2->_value) << std::endl;
                }
                else
                    ++first1;
                ++first2;
            }
        }
        while(first2 != last2)
        {
            result << fmt::format("{},,{}", first2->_id, first2->_value) << std::endl;
            ++first2;
            continue;
        }
        result << "OK" << std::endl;
        return result.str();
    }

    std::string intersection()
    {
        std::stringstream result;
        std::unique_lock lock(_mtx);
        auto first1 = _tables[0].begin();
        auto last1 = _tables[0].end();
        auto first2 = _tables[1].begin();
        auto last2 = _tables[1].end();
        while (first1 != last1 && first2 != last2)
        {
            if (*first1 < *first2)
            {
                ++first1;
            }
            else
            {
                if (!(*first2 < *first1))
                {
                    result << fmt::format("{},{},{}", first1->_id, first1->_value, first2->_value) << std::endl;
                    ++first1;
                }
                ++first2;
            }
        }
        result << "OK" << std::endl;
        return result.str();
    }
};