#include <algorithm>
#include <boost/crc.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
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

        std::size_t block_size = vm["block-size"].as<std::size_t>();
        std::vector<char> block(block_size);
        for (auto& file : files)
        {
            fmt::println("{}", file.path());
            
            if (file.is_open())
            {
                /////MD5/////
                auto _md5 = ud::md5();
                /////SHA1/////
                auto _sha1 = ud::sha1();
                /////CRC32/////
                boost::crc_32_type crc32;
                /////CRC16/////
                boost::crc_16_type crc16;
                while (!file.eof())
                {
                    std::fill(block.begin(), block.end(), '\0');
                    file.read(block.data(), block_size);
                    fmt::println("{}", block);
                    _md5.process_bytes(block.data(), block_size);
                    _sha1.process_bytes(block.data(), block_size);
                    crc32.process_bytes(block.data(), block_size);
                    crc16.process_bytes(block.data(), block_size);
                }
                file.close();
                ud::md5::digest_type md5digest;
                _md5.get_digest(md5digest);
                std::array<uint, 4> md5da;
                std::memcpy(md5da.data(), md5digest, sizeof(md5digest));
                fmt::print("MD5: ");
                for (auto i : md5da)
                {
                    fmt::print("{0:x}", i);
                }
                fmt::println("");
                ud::sha1::digest_type sha1digest;
                _sha1.get_digest(sha1digest);
                std::array<uint, 5> sha1da;
                std::memcpy(sha1da.data(), sha1digest, sizeof(sha1digest));
                fmt::print("SHA1: ");
                for (auto i : sha1da)
                {
                    fmt::print("{0:x}", i);
                }
                fmt::println("");
                fmt::println("CRC32: {0:x}", crc32.checksum());
                fmt::println("CRC16: {0:x}", crc16.checksum());
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