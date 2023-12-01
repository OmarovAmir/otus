#pragma once

#include <boost/filesystem.hpp>
#include <iostream>
namespace fs = boost::filesystem;

class FileReader : public std::ifstream
{
    fs::path _path;

  public:
    explicit FileReader(fs::path path, std::ios_base::openmode mode)
        : std::ifstream(path, mode)
        , _path{path}
    {}

    std::string path() const { return _path.string(); }
};