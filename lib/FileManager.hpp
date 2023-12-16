#pragma once

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
