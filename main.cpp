// #include <async.hpp>
// #include <iostream>
// #include <vector>

// int main(int argc, char** argv)
// {
//     if (argc != 2)
//     {
//         std::cout << "Usage: bulk_server batchSize" << std::endl;
//         return 0;
//     }
//     auto size = static_cast<std::size_t>(std::atol(argv[1]));
//     {
//         auto handle_0 = connect(size);
//         auto handle_1 = connect(size);
//         for (auto x = 0; x < 10000; ++x)
//         {
//             std::string cmd_0 = "cmd0_" + std::to_string(x);
//             std::string cmd_1 = "cmd1_" + std::to_string(x);
//             receive(handle_0, cmd_0.data(), cmd_0.size());
//             receive(handle_1, cmd_1.data(), cmd_1.size());
//         }
//         disconnect(handle_0);
//         disconnect(handle_1);
//     }
//     {
//         std::vector<std::string> cmds = {
//             "cmd1",
//             "cmd2",
//             "cmd3",
//             "cmd4",
//             "cmd5",
//         };
//         auto handle = connect(size);
//         for (const auto& cmd: cmds)
//         {
//             receive(handle, cmd.data(), cmd.size());
//         }
//         disconnect(handle);
//     }
//     {
//         std::vector<std::string> cmds = {
//             "cmd1",
//             "cmd2",
//             "{",
//             "cmd3",
//             "cmd4",
//             "}",
//             "{",
//             "cmd5",
//             "cmd6",
//             "{",
//             "cmd7",
//             "cmd8",
//             "}",
//             "cmd9",
//             "}",
//             "{",
//             "cmd10",
//             "cmd11"
//         };
//         auto handle = connect(size);
//         for (const auto& cmd: cmds)
//         {
//             receive(handle, cmd.data(), cmd.size());
//         }
//         disconnect(handle);
//     }
//     return 0;
// }

#include <Server.hpp>
#include <iostream>
#include <memory>

namespace asio = boost::asio;
using tcp = asio::ip::tcp;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;
    try
    {
        auto server = std::make_shared<Server>(9000, 3);
        server->run();
        return EXIT_SUCCESS;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Fatal error \"" << ex.what() << "\"." << std::endl;
    }
    catch (...)
    {
        std::cerr << "Fatal UNKNOWN error." << std::endl;
    }
    return 0;
}