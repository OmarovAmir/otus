#pragma once

#include <IHasher.hpp>
#include <boost/crc.hpp>
#include <memory>

class CRC32Hasher : public IHasher
{
    std::unique_ptr<boost::crc_32_type> crc32;

  public:
    void hash_bytes(void const* buffer, std::size_t byte_count) override final
    {
        init();
        crc32->process_bytes(buffer, byte_count);
    }

    std::vector<uint> getHash() const override final
    {
        std::vector<uint> result;
        result.push_back(crc32->checksum());
        return result;
    }

    void reset() override final { crc32->reset(); };

  private:
    void init()
    {
        if (!crc32)
        {
            crc32 = std::make_unique<boost::crc_32_type>();
        }
    }
};