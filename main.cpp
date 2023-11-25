#include <boost/program_options.hpp>
#include <fmt/ranges.h>
#include <iostream>
namespace po = boost::program_options;
using paths = std::vector<std::string>;

template <typename T> auto make_paths_notifier(const std::string& str)
{
    return [&str](T param) { fmt::println("{}: {}", str, param); };
}

int main(int argc, char** argv)
{
    try
    {
        po::options_description opts{"Options"};
        opts.add_options()("help,h", "this screen")("include-paths,i",
                                                    po::value<paths>()->multitoken()->composing()->required()->notifier(
                                                        make_paths_notifier<paths>("Include paths")),
                                                    "include paths")(
            "exclude-paths,e",
            po::value<paths>()->multitoken()->composing()->notifier(make_paths_notifier<paths>("Exclude paths")),
            "exclude paths")(
            "depth,d", po::value<std::size_t>()->default_value(0)->notifier(make_paths_notifier<std::size_t>("Depth")),
            "scan depth");

        po::variables_map vm;
        store(parse_command_line(argc, argv, opts), vm);

        if (vm.count("help"))
        {
            opts.print(std::cout);
            return 0;
        }

        po::notify(vm);
    }
    catch (std::exception& e)
    {
        fmt::println("Error: {}", e.what());
        return 0;
    }
    catch (...)
    {
        fmt::println("Unknown error!");
        return 0;
    }
    return 0;
}