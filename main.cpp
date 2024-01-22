#include <Server.hpp>
#include <fmt/format.h>
#include <iostream>
#include <stdexcept>
#include <Listener.hpp>
#include <ListenerManager.hpp>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: adapter port" << std::endl;
        return 0;
    }
    auto port = static_cast<std::size_t>(std::atol(argv[1]));
    try
    {
        auto mng = ListenerManager(4, {port, 2222, 6666, 5555, 4444});
        mng.run();
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