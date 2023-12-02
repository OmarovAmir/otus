#include <filter.hpp>
#include <iostream>
#include <options.hpp>
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

        paths include = vm["include-paths"].as<paths>();
        paths exclude;
        patterns ptrns;
        if (vm.count("exclude-paths"))
        {
            exclude = vm["exclude-paths"].as<paths>();
        }
        if (vm.count("patterns"))
        {
            ptrns = vm["patterns"].as<patterns>();
        }
        std::size_t depth = vm["depth"].as<std::size_t>();
        std::size_t mfs = vm["min-file-size"].as<std::size_t>();

        auto files = filter::filter(include, exclude, ptrns, depth, mfs);
        auto hasher = options::get_hasher(vm["hash-algorithm"].as<options::HashAlgorithm>());

        std::size_t block_size = vm["block-size"].as<std::size_t>();
        std::vector<char> block(block_size);
        for (auto& file : files)
        {
            fmt::println("{}", file.path());

            if (file.is_open())
            {
                while (!file.eof())
                {
                    std::fill(block.begin(), block.end(), '\0');
                    file.read(block.data(), block_size);
                    hasher->hash_bytes(block.data(), block_size);
                }
                file.close();
                fmt::println("HASH: {0:x}", fmt::join(hasher->getHash(), ""));
                hasher->reset();
            }
            else
            {
                fmt::println("Не удалось прочитать файл");
            }
        }
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