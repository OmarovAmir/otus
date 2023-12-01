#include <algorithm>
#include <boost/crc.hpp>
#include <MD5Hasher.hpp>
#include <SHA1Hasher.hpp>
#include <CRC16Hasher.hpp>
#include <CRC32Hasher.hpp>
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
                MD5Hasher md5;
                /////SHA1/////
                SHA1Hasher sha1;
                /////CRC32/////
                CRC32Hasher crc32;
                /////CRC16/////
                CRC16Hasher crc16;
                while (!file.eof())
                {
                    std::fill(block.begin(), block.end(), '\0');
                    file.read(block.data(), block_size);
                    md5.hash_bytes(block.data(), block_size);
                    sha1.hash_bytes(block.data(), block_size);
                    crc32.hash_bytes(block.data(), block_size);
                    crc16.hash_bytes(block.data(), block_size);
                }
                file.close();
                fmt::println("MD5: {0:x}", fmt::join(md5.getHash(), ""));
                fmt::println("SHA1: {0:x}", fmt::join(sha1.getHash(), ""));
                fmt::println("CRC32: {0:x}", fmt::join(crc32.getHash(), ""));
                fmt::println("CRC16: {0:x}", fmt::join(crc16.getHash(), ""));
                fmt::println("MD5: {0:x}", fmt::join(md5.getHash(), ""));
                fmt::println("SHA1: {0:x}", fmt::join(sha1.getHash(), ""));
                fmt::println("CRC32: {0:x}", fmt::join(crc32.getHash(), ""));
                fmt::println("CRC16: {0:x}", fmt::join(crc16.getHash(), ""));
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