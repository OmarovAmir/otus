#include <Server.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Usage: join_server port batchSize" << std::endl;
        return 0;
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