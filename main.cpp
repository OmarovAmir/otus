#include <boost/bimap.hpp>
#include <boost/program_options.hpp>
#include <fmt/ranges.h>
#include <iostream>
#include <map>
namespace po = boost::program_options;
using paths = std::vector<std::string>;

namespace {

template <typename T> auto make_paths_notifier(const std::string& param_name)
{
    return [param_name](T param) { fmt::println("{}: {}", param_name, param); };
}

enum class HashAlgorithm
{
    CRC16,
    CRC32,
    MD5
};

template <typename L, typename R>
boost::bimap<boost::bimaps::set_of<L>, boost::bimaps::set_of<R>>
makeBimap(const std::initializer_list<typename boost::bimap<L, R>::value_type>& list)
{
    return boost::bimap<boost::bimaps::set_of<L>, boost::bimaps::set_of<R>>(list.begin(), list.end());
}

const boost::bimap<boost::bimaps::set_of<HashAlgorithm>, boost::bimaps::set_of<std::string>> hashAlgorithmStringBimap =
    makeBimap<HashAlgorithm, std::string>({
        {HashAlgorithm::CRC16, std::string("crc16")},
        {HashAlgorithm::CRC32, std::string("crc32")},
        {HashAlgorithm::MD5, std::string("md5")},
    });

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

[[maybe_unused]] std::istream& operator>>(std::istream& in, HashAlgorithm& hashAlgorithmOption)
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

[[maybe_unused]] std::ostream& operator<<(std::ostream& out, const HashAlgorithm& hashAlgorithmOption)
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

}; // namespace

int main(int argc, char** argv)
{
    try
    {
        po::options_description opts{"Options"};
        opts.add_options()("help,h", "this screen")("include-paths,i",
                                                    po::value<paths>()->required()->multitoken()->composing()->notifier(
                                                        make_paths_notifier<paths>("Include paths")),
                                                    "include paths")(
            "exclude-paths,e",
            po::value<paths>()->multitoken()->composing()->notifier(make_paths_notifier<paths>("Exclude paths")),
            "exclude paths")(
            "depth,d", po::value<std::size_t>()->default_value(0)->notifier(make_paths_notifier<std::size_t>("Depth")),
            "scan depth")(
            "min-file-size,s",
            po::value<std::size_t>()->default_value(1)->notifier(make_paths_notifier<std::size_t>("Minimum file size")),
            "minimum file size")(
            "pattern,p", po::value<std::string>()->notifier(make_paths_notifier<std::string>("File name pattern")),
            "file name pattern")(
            "block-size,b",
            po::value<std::size_t>()->default_value(5)->notifier(make_paths_notifier<std::size_t>("Block size")),
            "block size")("hash-algorithm,a",
                          po::value<HashAlgorithm>()
                              ->default_value(HashAlgorithm::CRC16)
                              ->notifier(make_paths_notifier<HashAlgorithm>("Hash algorithm")),
                          "hash algorithm");

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