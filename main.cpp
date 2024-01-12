#include <Server.hpp>
#include <fmt/format.h>
#include <iostream>
#include <sqlite3.h>
#include <stdexcept>

/*
TODO вернуть логику запуска DataExtractorManager и выполнять запросы в отдельном потоке при их наличии в очереди запросов
*/

[[noreturn]] void sqlite_throw(int code, const char* msg = "")
{
    throw std::runtime_error{fmt::format("SQL Method failed: {} {}", sqlite3_errstr(code), msg)};
}

int sqlite_check(int code, const char* msg = "", int expected = SQLITE_OK)
{
    if (code != expected)
    {
        sqlite_throw(code, msg);
    }
    return code;
}

template <typename F> struct final_action
{
    final_action(F&& action)
        : action{std::move(action)}
    {}

    ~final_action() { action(); }
    F action;
};

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: join_server port batchSize" << std::endl;
        return 0;
    }

    sqlite3* db;
    sqlite_check(sqlite3_open("data.db", &db));
    final_action sqlite_deleter{[db]() { sqlite_check(sqlite3_close(db)); }};

    auto sql = "CREATE TABLE IF NOT EXISTS {}"
               "(id INTEGER PRIMARY KEY,"
               "name TEXT NOT NULL);";
    auto create_A = fmt::format(sql, "A");
    auto create_B = fmt::format(sql, "B");
    {
        sqlite3_stmt* create_table_stmt;
        const char* stmt_tail;
        if (sqlite3_prepare_v2(db, create_A.data(), create_A.size(), &create_table_stmt, &stmt_tail) != SQLITE_OK)
        {
            fmt::println("Error preparing statement: {}", sqlite3_errmsg(db));
            return -1;
        }

        int res;
        do
        {
            res = sqlite3_step(create_table_stmt);
            if (res == SQLITE_ROW)
            {}
            else if (res == SQLITE_DONE)
            {
                // ok
            }
            else
            {
                sqlite_throw(res);
            }
        }
        while (res != SQLITE_DONE);
        sqlite_check(sqlite3_finalize(create_table_stmt));
    }
    {
        sqlite3_stmt* create_table_stmt;
        const char* stmt_tail;
        if (sqlite3_prepare_v2(db, create_B.data(), create_B.size(), &create_table_stmt, &stmt_tail) != SQLITE_OK)
        {
            fmt::println("Error preparing statement: {}", sqlite3_errmsg(db));
            return -1;
        }

        int res;
        do
        {
            res = sqlite3_step(create_table_stmt);
            if (res == SQLITE_ROW)
            {}
            else if (res == SQLITE_DONE)
            {
                // ok
            }
            else
            {
                sqlite_throw(res);
            }
        }
        while (res != SQLITE_DONE);
        sqlite_check(sqlite3_finalize(create_table_stmt));
    }

    auto port = static_cast<std::size_t>(std::atol(argv[1]));
    auto size = static_cast<std::size_t>(std::atol(argv[2]));
    try
    {
        auto server = Server::create(port, size);
        server->run();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Fatal error \"" << ex.what() << "\"." << std::endl;
    }
    catch (...)
    {
        std::cerr << "Fatal UNKNOWN error." << std::endl;
    }
    return 0;
}