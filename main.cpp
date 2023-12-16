#include <async.h>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: bulk_mtd batchSize" << std::endl;
        return 0;
    }

    auto handle = connect(static_cast<std::size_t>(std::atol(argv[1])));
    auto handle2 = connect(static_cast<std::size_t>(std::atol(argv[1])));
    std::string str("cmd1");
    receive(handle, str.data(), str.size());
    receive(handle2, str.data(), str.size());
    disconnect(handle);
    disconnect(handle2);
    return 0;
}
