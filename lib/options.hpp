#pragma once

#include <boost/bimap.hpp>
#include <boost/program_options.hpp>
#include <fmt/ranges.h>

namespace po = boost::program_options;
using paths = std::vector<std::string>;
using patterns = std::vector<std::string>;

namespace options {

template <typename T> auto make_paths_notifier(const std::string& param_name)
{
    return [param_name](T param) { fmt::println("{}: {}", param_name, param); };
}

enum class HashAlgorithm
{
    CRC16,
    CRC32,
    MD5,
    SHA1
};

template <typename L, typename R>
boost::bimap<boost::bimaps::set_of<L>, boost::bimaps::set_of<R>>
makeBimap(const std::initializer_list<typename boost::bimap<L, R>::value_type>& list)
{
    return boost::bimap<boost::bimaps::set_of<L>, boost::bimaps::set_of<R>>(list.begin(), list.end());
}

const boost::bimap<boost::bimaps::set_of<HashAlgorithm>, boost::bimaps::set_of<std::string>> hashAlgorithmStringBimap =
    makeBimap<HashAlgorithm, std::string>({{HashAlgorithm::CRC16, std::string("crc16")},
                                           {HashAlgorithm::CRC32, std::string("crc32")},
                                           {HashAlgorithm::MD5, std::string("md5")},
                                           {HashAlgorithm::SHA1, std::string("sha1")}});

const std::string& typeToString(const HashAlgorithm type)
{
    auto iter = hashAlgorithmStringBimap.left.find(type);
    if (iter == hashAlgorithmStringBimap.left.end())
        throw std::logic_error("No such type!");
    return iter->second;
}

HashAlgorithm typeFromString(const std::string& type)
{
    auto iter = hashAlgorithmStringBimap.right.find(type);
    if (iter == hashAlgorithmStringBimap.right.end())
        throw std::logic_error("No such type!");
    return iter->second;
}

auto format_as(const HashAlgorithm& hashAlgorithm) { return typeToString(hashAlgorithm); }

std::istream& operator>>(std::istream& in, HashAlgorithm& hashAlgorithmOption)
{
    try
    {
        std::string hashAlgorithm;
        in >> hashAlgorithm;
        hashAlgorithmOption = typeFromString(hashAlgorithm);
    }
    catch (std::exception& e)
    {
        in.setstate(std::ios_base::failbit);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const HashAlgorithm& hashAlgorithmOption)
{
    try
    {
        out << typeToString(hashAlgorithmOption);
    }
    catch (std::exception& e)
    {
        out.setstate(std::ios_base::failbit);
    }
    return out;
}

po::options_description opts_init()
{
    po::options_description opts{"Options"};
    auto optsInit = opts.add_options();
    // Help
    optsInit("help,h", "this screen");
    // Include paths
    optsInit("include-paths,i",
             po::value<paths>()->required()->multitoken()->composing()->notifier(
                 make_paths_notifier<paths>("Include paths")),
             "include paths");
    // Exclude paths
    optsInit("exclude-paths,e",
             po::value<paths>()->multitoken()->composing()->notifier(make_paths_notifier<paths>("Exclude paths")),
             "exclude paths");
    // Depth
    optsInit("depth,d", po::value<std::size_t>()->default_value(0)->notifier(make_paths_notifier<std::size_t>("Depth")),
             "scan depth");
    // Minimum file size
    optsInit(
        "min-file-size,s",
        po::value<std::size_t>()->default_value(1)->notifier(make_paths_notifier<std::size_t>("Minimum file size")),
        "minimum file size");
    // File name pattern
    optsInit(
        "patterns,p",
        po::value<patterns>()->multitoken()->composing()->notifier(make_paths_notifier<patterns>("File name patterns")),
        "file name pattern");
    // Block size
    optsInit("block-size,b",
             po::value<std::size_t>()->default_value(5)->notifier(make_paths_notifier<std::size_t>("Block size")),
             "block size");
    // Hash algorithm
    optsInit("hash-algorithm,a",
             po::value<HashAlgorithm>()
                 ->default_value(HashAlgorithm::CRC16)
                 ->notifier(make_paths_notifier<HashAlgorithm>("Hash algorithm")),
             "hash algorithm");
    return opts;
}

po::variables_map parse_options(int argc, char** argv, const po::options_description& opts)
{
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opts), vm);
    po::notify(vm);
    return vm;
}

} // namespace options
