#include <DataExtractor.hpp>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: bulk_mtd batchSize" << std::endl;
        return 0;
    }
    DataExtractor dxtr{static_cast<std::size_t>(std::atol(argv[1]))};
    dxtr.run();
    return 0;
}
