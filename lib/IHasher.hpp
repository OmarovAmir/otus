#pragma once

#include <vector>

class IHasher
{
  public:
    virtual void hash_bytes(void const* buffer, std::size_t byte_count) = 0;
    virtual std::vector<unsigned int> getHash() const = 0;
    virtual void reset() = 0;
};