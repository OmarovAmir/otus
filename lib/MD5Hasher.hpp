#pragma once

#include <IHasher.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <cstring>
#include <memory>
namespace ud = boost::uuids::detail;

class MD5Hasher : public IHasher
{
    std::unique_ptr<ud::md5> md5;

  public:
    void hash_bytes(void const* buffer, std::size_t byte_count) override final
    {
        init();
        md5->process_bytes(buffer, byte_count);
    }

    std::vector<uint> getHash() override final
    {
        std::vector<uint> result;
        if (md5)
        {
            ud::md5 md5Copy;
            std::memcpy(&md5Copy, md5.get(), sizeof(md5Copy));
            ud::md5::digest_type digest = {0};
            md5Copy.get_digest(digest);
            for (const auto& i : digest)
            {
                result.push_back(i);
            }
        }
        return result;
    }

    void reset() override final { md5.release(); };

  private:
    void init()
    {
        if (!md5)
        {
            md5 = std::make_unique<ud::md5>();
        }
    }
};