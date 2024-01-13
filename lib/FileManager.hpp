#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

/// @brief Файловый менеджер
class FileManager
{
  public:
    /// @brief Сохранить в файл
    /// @param path Путь к файлу
    /// @param data Данные
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
