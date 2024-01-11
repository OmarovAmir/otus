#include <Server.hpp>
#include <iostream>
#include <sqlite3.h>
#include <fmt/format.h>

[[noreturn]] void sqlite_throw(int code, const char* msg = "")
{
    throw std::runtime_error{fmt::println("SQL Method failed: {} {}", sqlite3_errstr(code), msg)};
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
    if (argc != 2)
    {
        std::cout << "Usage: join_server port" << std::endl;
        return 0;
    }

    sqlite3* db;
    sqlite_check(sqlite3_open("db", &db));
    final_action sqlite_deleter{[db]()
                                {
                                    sqlite_check(sqlite3_close(db));
                                }};

    std::string_view sql{
      "CREATE TABLE IF NOT EXISTS user"
      "(id INTEGER PRIMARY KEY,"
      "balance UNSIGNED BIG INT CHECK (balance >= 0),"
      "name VARCHAR(255) NOT NULL);"};
    sqlite3_stmt* create_table_stmt;
    const char* stmt_tail;
    if (sqlite3_prepare_v2(db, sql.data(), sql.size(), &create_table_stmt,
                            &stmt_tail) != SQLITE_OK) {
        fmt::println("Error preparing statement: {}",
                                sqlite3_errmsg(db));
        return -1;
    }

    auto port = static_cast<std::size_t>(std::atol(argv[1]));
    try
    {
        auto server = Server::create(port);
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