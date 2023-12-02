#pragma once

#include <IHasher.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include <options.hpp>
namespace fs = boost::filesystem;

class FileReader : public std::ifstream
{
    fs::path _path;
    std::shared_ptr<IHasher> _hasher;

  public:
    explicit FileReader(fs::path path, std::ios_base::openmode mode, const options::HashAlgorithm& hashAlgorithm)
        : std::ifstream(path, mode)
        , _path{path}
        , _hasher{options::get_hasher(hashAlgorithm)}
    {}

    std::string path() const { return _path.string(); }

    void hashNext(const std::size_t& blockSize)
    {
        std::vector<char> block(blockSize);
        read(block.data(), blockSize);
        if (_hasher)
        {
            _hasher->hash_bytes(block.data(), blockSize);
        }
    }

    std::vector<uint> getHash() const
    {
        if (_hasher)
        {
            return _hasher->getHash();
        }
        return {};
    }

    bool operator<(const FileReader& other) const { return (getHash() < other.getHash()); }
};