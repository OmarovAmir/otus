#pragma once

#include <FileReader.hpp>
#include <algorithm>
#include <list>

namespace search {

void searchDublicates(std::list<FileReader>& files, const std::size_t& blockSize, bool verbose)
{
    while ((files.size() > 1) &&
           !std::all_of(files.begin(), files.end(), [](const FileReader& file) { return file.isEof(); }))
    {
        for (auto& file : files)
        {
            if (file.isOpen())
            {
                file.hashNext(blockSize);
            }
        }
        for (auto it = files.begin(); it != files.end();)
        {
            auto count =
                std::count_if(files.begin(), files.end(),
                              [hash = it->getHash()](const FileReader& file) { return (hash == file.getHash()); });
            if (count > 1)
            {
                ++it;
            }
            else
            {
                it = files.erase(it);
            }
        }
    }
    if (files.size() > 1)
    {
        files.sort();
        auto hash = files.front().getHash();
        if (verbose)
        {
            fmt::println("Hash: {0:x}", fmt::join(hash, ""));
        }
        for (const auto& file : files)
        {
            if (hash != file.getHash())
            {
                hash = file.getHash();
                fmt::println("");
                if (verbose)
                {
                    fmt::println("Hash: {0:x}", fmt::join(hash, ""));
                }
            }
            fmt::println("{}", file.path());
        }
    }
}

}; // namespace search