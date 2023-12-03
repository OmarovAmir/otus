#pragma once

#include <FileReader.hpp>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <list>
#include <options.hpp>
namespace fs = boost::filesystem;

namespace filter {

bool checkDirectory(const paths& exclude, const std::string path_str)
{
    return std::any_of(exclude.begin(), exclude.end(),
                       [&path_str](std::string ex) { return (path_str.find(ex) != std::string::npos); });
}

bool checkFile(const patterns& ptrns, const std::size_t& mfs, const fs::path path)
{
    return ((ptrns.empty() || std::any_of(ptrns.begin(), ptrns.end(),
                                          [path_str = path.filename().string()](std::string pt) {
                                              return (boost::to_lower_copy(path_str).find(boost::to_lower_copy(pt)) !=
                                                      std::string::npos);
                                          })) &&
            (fs::file_size(path) > mfs));
}

std::list<FileReader> filter(const paths& include, const paths& exclude, const patterns& ptrns,
                             const std::size_t& depth, const std::size_t& mfs, options::HashAlgorithm& hashAlgorithm)
{
    std::list<FileReader> files;
    for (const auto& path_str : include)
    {
        if (checkDirectory(exclude, path_str))
        {
            continue;
        }
        fs::path path{path_str};
        if (fs::exists(path))
        {
            if (fs::is_regular_file(path))
            {
                if (checkFile(ptrns, mfs, path))
                {
                    files.emplace_back(path, std::ios::binary, hashAlgorithm);
                }
            }
            else if (fs::is_directory(path))
            {
                for (auto ri = fs::recursive_directory_iterator(path); ri != fs::end(ri); ++ri)
                {
                    if (checkDirectory(exclude, ri->path().string()))
                    {
                        continue;
                    }
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
                        if (checkFile(ptrns, mfs, ri->path()))
                        {
                            files.emplace_back(ri->path(), std::ios::binary, hashAlgorithm);
                        }
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
    return files;
}

}; // namespace filter