#include <async.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: bulk_mtd batchSize" << std::endl;
        return 0;
    }
    auto size = static_cast<std::size_t>(std::atol(argv[1]));
    auto handle_0 = connect(size);
    auto handle_1 = connect(size);
    for (auto x = 0; x < 10000; ++x)
    {
        std::string cmd_0 = "cmd0_" + std::to_string(x);
        std::string cmd_1 = "cmd1_" + std::to_string(x);
        receive(handle_0, cmd_0.data(), cmd_0.size());
        receive(handle_1, cmd_1.data(), cmd_1.size());
    }
    disconnect(handle_0);
    disconnect(handle_1);
    return 0;
}
