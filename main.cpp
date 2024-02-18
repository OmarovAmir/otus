#include <Listener.hpp>
#include <ListenerManager.hpp>
#include <Options.hpp>
#include <fmt/format.h>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv)
{
    try
    {
        po::options_description opts = opts_init();
        po::variables_map vm = parse_options(argc, argv, opts);
        if (vm.count("help"))
        {
            opts.print(std::cout);
            return 0;
        }
        auto ports = vm["ports"].as<std::vector<std::size_t>>();
        auto contextNumber = vm["context-number"].as<std::size_t>();
        auto severity = vm["logger-severity"].as<Logger::Severity>();
        Logger::getInstance().setSeverity(severity);
        auto mng = ListenerManager(contextNumber, ports);
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