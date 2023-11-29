#include <boost/filesystem.hpp>
#include <iostream>
#include <list>
#include <options.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <boost/crc.hpp>
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
        
        for (const auto& file : files)
        {
            fmt::println("{}", file.string());
            {
                std::ifstream file_stream(file, std::ios::binary);
                if (file_stream.is_open())
                {
                    /////MD5/////
                    auto _md5 = ud::md5();
                    for (auto i = std::istreambuf_iterator<char>(file_stream); i != std::istreambuf_iterator<char>(); ++i)
                    {
                        _md5.process_byte(*i);
                    }
                    file_stream.close();
                    ud::md5::digest_type digest;
                    _md5.get_digest(digest);
                    std::array<uint,4> da;
                    std::memcpy(da.data(), digest, sizeof(digest));
                    fmt::print("MD5: ");
                    for(auto i: da)
                    {
                        fmt::print("{0:x}", i);
                    }
                    fmt::println("");
                }
                else
                {
                    fmt::println("Не удалось прочитать файл");
                }
            }
            {
                std::ifstream file_stream(file, std::ios::binary);
                if (file_stream.is_open())
                {
                    /////SHA1/////
                    auto _sha1 = ud::sha1();
                    for (auto i = std::istreambuf_iterator<char>(file_stream); i != std::istreambuf_iterator<char>(); ++i)
                    {
                        _sha1.process_byte(*i);
                    }
                    file_stream.close();
                    ud::sha1::digest_type digest;
                    _sha1.get_digest(digest);
                    std::array<uint, 5> da;
                    std::memcpy(da.data(), digest, sizeof(digest));
                    fmt::print("SHA1: ");
                    for(auto i: da)
                    {
                        fmt::print("{0:x}", i);
                    }
                    fmt::println("");
                }
                else
                {
                    fmt::println("Не удалось прочитать файл");
                }
            }
            {
                std::ifstream file_stream(file, std::ios::binary);
                if (file_stream.is_open())
                {
                    /////CRC32/////
                    boost::crc_32_type crc32;
                    for (auto i = std::istreambuf_iterator<char>(file_stream); i != std::istreambuf_iterator<char>(); ++i)
                    {
                        crc32.process_byte(*i);
                    }
                    file_stream.close();
                    fmt::println("CRC32: {0:x}", crc32.checksum());
                }
                else
                {
                    fmt::println("Не удалось прочитать файл");
                }
            }
            {
                std::ifstream file_stream(file, std::ios::binary);
                if (file_stream.is_open())
                {
                    /////CRC16/////
                    boost::crc_16_type crc16;
                    for (auto i = std::istreambuf_iterator<char>(file_stream); i != std::istreambuf_iterator<char>(); ++i)
                    {
                        crc16.process_byte(*i);
                    }
                    file_stream.close();
                    fmt::println("CRC16: {0:x}", crc16.checksum());
                }
                else
                {
                    fmt::println("Не удалось прочитать файл");
                }
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