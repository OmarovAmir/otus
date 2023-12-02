#pragma once

#include <IHasher.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <cstring>
#include <memory>
namespace ud = boost::uuids::detail;

class SHA1Hasher : public IHasher
{
    std::unique_ptr<ud::sha1> sha1;

  public:
    void hash_bytes(void const* buffer, std::size_t byte_count) override final
    {
        init();
        sha1->process_bytes(buffer, byte_count);
    }

    std::vector<uint> getHash() const override final
    {
        std::vector<uint> result;
        if (sha1)
        {
            ud::sha1 sha1Copy;
            std::memcpy(&sha1Copy, sha1.get(), sizeof(sha1Copy));
            ud::sha1::digest_type digest = {0};
            sha1Copy.get_digest(digest);
            for (const auto& i : digest)
            {
                result.push_back(i);
            }
        }
        return result;
    }

    void reset() override final { sha1.release(); };

  private:
    void init()
    {
        if (!sha1)
        {
            sha1 = std::make_unique<ud::sha1>();
        }
    }
};