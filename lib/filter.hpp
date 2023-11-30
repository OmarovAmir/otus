#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <list>
#include <options.hpp>
namespace fs = boost::filesystem;

namespace filter {
std::list<fs::path> filter(const paths& include, const paths& exclude, const patterns& ptrns, const std::size_t& depth,
                           const std::size_t& mfs)
{
    std::list<fs::path> files;
    for (const auto& path_str : include)
    {
        if (std::any_of(exclude.begin(), exclude.end(),
                        [&path_str](std::string ex) { return (path_str.find(ex) != std::string::npos); }))
        {
            continue;
        }
        fs::path path{path_str};
        if (fs::exists(path))
        {
            if (fs::is_regular_file(path))
            {
                if ((ptrns.empty() || std::any_of(ptrns.begin(), ptrns.end(),
                                                  [path_str = path.filename().string()](std::string pt) {
                                                      return (boost::to_lower_copy(path_str).find(
                                                                  boost::to_lower_copy(pt)) != std::string::npos);
                                                  })) &&
                    (fs::file_size(path) > mfs))
                {
                    files.push_back(path);
                }
            }
            else if (fs::is_directory(path))
            {
                for (auto ri = fs::recursive_directory_iterator(path); ri != fs::end(ri); ++ri)
                {
                    if (std::any_of(exclude.begin(), exclude.end(),
                                    [&ri](std::string ex)
                                    { return (ri->path().string().find(ex) != std::string::npos); }))
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
                        if ((ptrns.empty() ||
                             std::any_of(ptrns.begin(), ptrns.end(),
                                         [path_str = ri->path().filename().string()](std::string pt) {
                                             return (boost::to_lower_copy(path_str).find(boost::to_lower_copy(pt)) !=
                                                     std::string::npos);
                                         })) &&
                            (fs::file_size(ri->path()) > mfs))
                        {
                            files.push_back(ri->path());
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

    for (const auto& file : files)
    {
        fmt::println("{}", file.string());
    }
    return files;
}
}; // namespace filter