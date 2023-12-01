#include <boost/filesystem.hpp>
#include <iostream>
namespace fs = boost::filesystem;

class file_reader : public std::ifstream
{
    fs::path _path;

  public:
    explicit file_reader(fs::path path, std::ios_base::openmode mode)
        : std::ifstream(path, mode)
        , _path{path}
    {}

    std::string path() const { return _path.string(); }
};