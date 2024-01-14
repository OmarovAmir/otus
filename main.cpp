#include <Server.hpp>
#include <fmt/format.h>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: join_server port" << std::endl;
        return 0;
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