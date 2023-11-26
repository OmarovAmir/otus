#include <iostream>
#include <options.hpp>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

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

        fs::path path = vm["include-paths"].as<paths>()[0];
        std::size_t depth = vm["depth"].as<std::size_t>();

        if (fs::exists(path))
        {
            if (fs::is_regular_file(path))
                std::cout << path << " size is " << fs::file_size(path) << '\n';

            else if (fs::is_directory(path))
            {
                std::cout << path << " is a directory containing:\n";

                for (auto ri = fs::recursive_directory_iterator(path); ri != fs::recursive_directory_iterator(); ++ri)
                {
                    if(ri.depth() > static_cast<int>(depth))
                    {
                        continue;
                    }
                    std::cout << "    " << ri->path() << '\n';
                }
            }
            else
                std::cout << path << " exists, but is not a regular file or directory\n";
        }
        else
            std::cout << path << " does not exist\n";
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