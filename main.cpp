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

        auto x = filter::filter(include, exclude, ptrns, depth, mfs);

        // std::size_t block_size = vm["block-size"].as<std::size_t>();
        // std::vector<char> block(block_size);
        // for (const auto& file : files)
        // {
        //     fmt::println("{}", file.string());
        //     {
        //         std::ifstream file_stream(file, std::ios::binary);
        //         if (file_stream.is_open())
        //         {
        //             /////MD5/////
        //             auto _md5 = ud::md5();
        //             while (!file_stream.eof())
        //             {
        //                 std::fill(block.begin(), block.end(), '\0');
        //                 file_stream.read(block.data(), block_size);
        //                 fmt::println("{}", block);
        //                 _md5.process_bytes(block.data(), block_size);
        //             }
        //             file_stream.close();
        //             ud::md5::digest_type digest;
        //             _md5.get_digest(digest);
        //             std::array<uint, 4> da;
        //             std::memcpy(da.data(), digest, sizeof(digest));
        //             fmt::print("MD5: ");
        //             for (auto i : da)
        //             {
        //                 fmt::print("{0:x}", i);
        //             }
        //             fmt::println("");
        //         }
        //         else
        //         {
        //             fmt::println("Не удалось прочитать файл");
        //         }
        //     }
        //     {
        //         std::ifstream file_stream(file, std::ios::binary);
        //         if (file_stream.is_open())
        //         {
        //             /////SHA1/////
        //             auto _sha1 = ud::sha1();
        //             while (!file_stream.eof())
        //             {
        //                 std::fill(block.begin(), block.end(), '\0');
        //                 file_stream.read(block.data(), block_size);
        //                 fmt::println("{}", block);
        //                 _sha1.process_bytes(block.data(), block_size);
        //             }
        //             file_stream.close();
        //             ud::sha1::digest_type digest;
        //             _sha1.get_digest(digest);
        //             std::array<uint, 5> da;
        //             std::memcpy(da.data(), digest, sizeof(digest));
        //             fmt::print("SHA1: ");
        //             for (auto i : da)
        //             {
        //                 fmt::print("{0:x}", i);
        //             }
        //             fmt::println("");
        //         }
        //         else
        //         {
        //             fmt::println("Не удалось прочитать файл");
        //         }
        //     }
        //     {
        //         std::ifstream file_stream(file, std::ios::binary);
        //         if (file_stream.is_open())
        //         {
        //             /////CRC32/////
        //             boost::crc_32_type crc32;
        //             while (!file_stream.eof())
        //             {
        //                 std::fill(block.begin(), block.end(), '\0');
        //                 file_stream.read(block.data(), block_size);
        //                 fmt::println("{}", block);
        //                 crc32.process_bytes(block.data(), block_size);
        //             }
        //             file_stream.close();
        //             fmt::println("CRC32: {0:x}", crc32.checksum());
        //         }
        //         else
        //         {
        //             fmt::println("Не удалось прочитать файл");
        //         }
        //     }
        //     {
        //         std::ifstream file_stream(file, std::ios::binary);
        //         if (file_stream.is_open())
        //         {
        //             /////CRC16/////
        //             boost::crc_16_type crc16;
        //             while (!file_stream.eof())
        //             {
        //                 std::fill(block.begin(), block.end(), '\0');
        //                 file_stream.read(block.data(), block_size);
        //                 fmt::println("{}", block);
        //                 crc16.process_bytes(block.data(), block_size);
        //             }
        //             file_stream.close();
        //             fmt::println("CRC16: {0:x}", crc16.checksum());
        //         }
        //         else
        //         {
        //             fmt::println("Не удалось прочитать файл");
        //         }
        //     }
        // }
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