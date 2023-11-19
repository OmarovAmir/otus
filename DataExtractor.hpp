#ifndef BC6E047B_B766_46E5_9261_E4634670DFDE
#define BC6E047B_B766_46E5_9261_E4634670DFDE

#include <string>
#include <iostream>

#include <CommandBatch.hpp>

class DataExtractor
{
  public:
    void run(int argc, char** argv)
    {
        if (argc != 2)
        {
            std::cout << "Usage: bulk batchSize" << std::endl;
            return;
        }
        CommandBatch cmdBatch{static_cast<std::size_t>(std::atol(argv[1]))};
        std::string input;
        while (std::getline(std::cin, input))
        {
            cmdBatch.add(input);
        }
    }
};

#endif /* BC6E047B_B766_46E5_9261_E4634670DFDE */
