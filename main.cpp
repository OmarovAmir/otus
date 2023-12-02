#include <filter.hpp>
#include <iostream>
#include <options.hpp>
#include <search.hpp>
namespace fs = boost::filesystem;
namespace ud = boost::uuids::detail;

int main(int argc, char** argv)
{
    try
    {
        po::options_description opts = options::opts_init();
        po::variables_map vm = options::parse_options(argc, argv, opts);
        if (vm.count("help"))
        {
            opts.print(std::cout);
            return 0;
        }

        paths include;
        paths exclude;
        patterns ptrns;
        if (vm.count("include-paths"))
        {
            include = vm["include-paths"].as<paths>();
        }
        if (vm.count("exclude-paths"))
        {
            exclude = vm["exclude-paths"].as<paths>();
        }
        if (vm.count("patterns"))
        {
            ptrns = vm["patterns"].as<patterns>();
        }
        auto depth = vm["depth"].as<std::size_t>();
        auto mfs = vm["min-file-size"].as<std::size_t>();
        auto block_size = vm["block-size"].as<std::size_t>();
        auto hashAlgorithm = vm["hash-algorithm"].as<options::HashAlgorithm>();

        auto files = filter::filter(include, exclude, ptrns, depth, mfs, hashAlgorithm);
        search::searchDublicates(files, block_size, vm.count("verbose"));
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