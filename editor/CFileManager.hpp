#ifndef A93F875F_5C70_4477_8DC9_C5884EBEF300
#define A93F875F_5C70_4477_8DC9_C5884EBEF300

#include <IGraphicPrimitive.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief Класс для импорта/экспорта данных
 *
 */
class CFileManager : std::enable_shared_from_this<CFileManager>
{
  public:
    std::shared_ptr<CFileManager> getptr()
    {
        return shared_from_this();
    }

    /**
     * @brief Импорт данных из файла
     *
     * @param[in] path Путь к файлу
     * @param[out] data Массив данных
     * @return true Успех
     * @return false Не успех
     */
    bool importData([[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] std::vector<IGraphicPrimitive>& data)
    {
        std::cout << "Импорта данных из файла " << path << std::endl;
        for (int i = 0; i < 10; ++i)
        {
            IGraphicPrimitive gp{"Линия", 0.0 + i, 0.0 + i + 1};
            std::string gpData = std::to_string(i) + ". Данные примитива"
            + " { " + gp.getName() + "; " + std::to_string(gp.getCoordinates().getX()) + "; " + std::to_string(gp.getCoordinates().getY()) + " }";
            gp.importData(gpData);
            data.push_back(gp);
        }
        return true;
    }

    /**
     * @brief Экспорт данных из файла
     *
     * @param[in] path Путь к файлу
     * @param[in] data Массив данных
     * @return true Успех
     * @return false Не успех
     */
    bool exportData([[maybe_unused]] const std::filesystem::path& path, [[maybe_unused]] const std::vector<IGraphicPrimitive>& data)
    {
        std::cout << "Экспорт данных в файл " << path << std::endl;
        int count = 0;
        for (auto& gp: data)
        {
            auto gpData = std::to_string(count) + ". " + gp.exportData();
            std::cout << gpData << std::endl;
            ++count;
        }
        return true;
    }
};

#endif /* A93F875F_5C70_4477_8DC9_C5884EBEF300 */
