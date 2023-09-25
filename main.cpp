#include "fast_allocator.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char const *argv[])
{
    try
    {
        std::cout << "Create the allocator" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}