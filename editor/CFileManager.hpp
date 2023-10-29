#ifndef A93F875F_5C70_4477_8DC9_C5884EBEF300
#define A93F875F_5C70_4477_8DC9_C5884EBEF300

#include <IGraphicPrimitive.hpp>
#include <filesystem>
#include <iostream>
#include <vector>

/**
 * @brief Класс для импорта/экспорта данных
 *
 */
class CFileManager
{
  public:
    /**
     * @brief Импорт данных из файла
     * 
     * @param[in] path Путь к файлу
     * @param[out] data Массив данных
     * @return true Успех
     * @return false Не успех
     */
    bool importData(const std::filesystem::path& path, std::vector<IGraphicPrimitive>& data)
    {
        std::cout << "Импорта данных из файла " << path << std::endl;
    }

    /**
     * @brief Экспорт данных из файла
     * 
     * @param[in] path Путь к файлу
     * @param[in] data Массив данных
     * @return true Успех
     * @return false Не успех
     */
    bool exportData(const std::filesystem::path& path, const std::vector<IGraphicPrimitive>& data)
    {
        std::cout << "Экспорт данных в файл " << path << std::endl;
    }
};

#endif /* A93F875F_5C70_4477_8DC9_C5884EBEF300 */
