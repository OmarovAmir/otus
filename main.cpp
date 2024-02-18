#include <Listener.hpp>
#include <ListenerManager.hpp>
#include <fmt/format.h>
#include <iostream>
#include <stdexcept>
#include <Logger.hpp>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: adapter port [port]" << std::endl;
        return 0;
    }

    std::vector<std::size_t> ports;

    for (auto p = 1; p < argc; ++p)
    {
        ports.push_back(static_cast<std::size_t>(std::atol(argv[p])));
    }

    try
    {
        Logger::getInstance().setSeverity(Logger::Severity::Debug);
        auto mng = ListenerManager(4, ports);
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