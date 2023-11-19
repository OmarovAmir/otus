#ifndef A3FE653B_7759_468C_A44A_5ABAD4FC2625
#define A3FE653B_7759_468C_A44A_5ABAD4FC2625

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

class FileManager
{
  public:
    static void save(const std::filesystem::path& path, const std::string& data)
    {
        std::ofstream logfile(path);
        if (logfile.is_open())
        {
            logfile << data;
            logfile.close();
        }
        else
        {
            std::cout << "Не удалось записать файл с содержимым: " << std::endl << data;
        }
    }
};

#endif /* A3FE653B_7759_468C_A44A_5ABAD4FC2625 */
