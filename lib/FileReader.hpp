#pragma once

#include <IHasher.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <memory>
#include <options.hpp>
namespace fs = boost::filesystem;

class FileReader
{
    fs::path _path;
    std::unique_ptr<IHasher> _hasher;
    std::unique_ptr<std::ifstream> _stream;

  public:
    explicit FileReader(fs::path path, std::ios_base::openmode mode, const options::HashAlgorithm& hashAlgorithm)
        : _path{path}
        , _hasher{options::get_hasher(hashAlgorithm)}
        , _stream{std::make_unique<std::ifstream>(path, mode)}
    {}

    std::string path() const { return _path.string(); }

    void hashNext(const std::size_t& blockSize)
    {
        if (_stream->eof())
        {
            return;
        }
        std::vector<char> block(blockSize);
        _stream->read(block.data(), blockSize);
        if ((0 != _stream->gcount()) && _hasher)
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

    bool isOpen() const { return _stream->is_open(); }

    bool isEof() const { return _stream->eof(); }

    bool operator<(const FileReader& other) const { return (getHash() < other.getHash()); }
};