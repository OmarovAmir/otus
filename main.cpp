#include <boost/filesystem.hpp>
#include <iostream>
#include <list>
#include <options.hpp>
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
        std::list<fs::path> files;
        std::size_t depth = vm["depth"].as<std::size_t>();
        for (const auto& path_str : vm["include-paths"].as<paths>())
        {
            fs::path path{path_str};
            if (fs::exists(path))
            {
                if (fs::is_regular_file(path))
                {
                    files.push_back(path);
                }
                else if (fs::is_directory(path))
                {
                    for (auto ri = fs::recursive_directory_iterator(path); ri != fs::end(ri); ++ri)
                    {
                        if (ri.depth() > static_cast<int>(depth))
                        {
                            ri.pop();
                            if (ri == fs::end(ri))
                            {
                                break;
                            }
                        }
                        if (fs::is_regular_file(ri->path()))
                        {
                            files.push_back(ri->path());
                        }
                    }
                }
                else
                {
                    fmt::println("{} does not file or directory", path.string());
                }
            }
            else
            {
                fmt::println("{} does not exist", path.string());
            }
        }

        for(const auto& file: files)
        {
            fmt::println("{}", file.string());
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