#pragma once

#include <IHasher.hpp>
#include <boost/crc.hpp>
#include <memory>

class CRC16Hasher : public IHasher
{
    std::unique_ptr<boost::crc_16_type> crc16;

  public:
    void hash_bytes(void const* buffer, std::size_t byte_count) override final
    {
        init();
        crc16->process_bytes(buffer, byte_count);
    }

    std::vector<uint> getHash() const override final
    {
        std::vector<uint> result;
        result.push_back(crc16->checksum());
        return result;
    }

    void reset() override final { crc16->reset(); };

  private:
    void init()
    {
        if (!crc16)
        {
            crc16 = std::make_unique<boost::crc_16_type>();
        }
    }
};