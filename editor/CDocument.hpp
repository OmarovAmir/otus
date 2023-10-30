#ifndef B26CA707_4BE2_49BC_A48F_D156D1030252
#define B26CA707_4BE2_49BC_A48F_D156D1030252

#include <CFileManager.hpp>
#include <IGraphicPrimitive.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

/**
 * @brief Класс представляющий абстракцию для работы с документами
 *
 */
class CDocument : std::enable_shared_from_this<CDocument>
{
  public:
    /**
     * @brief Конструктор
     *
     * @param[in] fileManager Указатель файловый менеджер
     * @param[in] path Путь к файлу представляющий документ
     */
    explicit CDocument(std::shared_ptr<CFileManager>& fileManager, std::filesystem::path path = "")
        : _fileManager{fileManager}, _path{path}
    {
        std::cout << "Конструктор документа" << std::endl;
        if (!_path.empty())
        {
            if (!_fileManager->importData(_path, _data))
            {
                std::cout << "При импорте документа произошла ошибка" << std::endl;
            }
        }
    }

    /**
     * @brief Деструктор
     *
     */
    ~CDocument()
    {
        std::cout << "Деструктор документа" << std::endl;
        if(!_path.empty())
        {
            if (!_fileManager->exportData(_path, _data))
            {
                std::cout << "При сохранении документа произошла ошибка" << std::endl;
            }
        }
    }

    /**
     * @brief Сохранить документв в файл
     *
     * @return true документ сохранился
     * @return false документ не сохранился
     */
    bool save()
    {
        std::cout << "Сохраняем документ" << std::endl;
        return _fileManager->exportData(_path, _data);
    }

    /**
     * @brief Добавить графические примитивы в документ
     *
     * @param[in] data Массив графических примитивов для добавления в документ
     * @return true массив графических примитивов добавился
     * @return false массив графических примитивов не добавился
     */
    bool add(const std::vector<IGraphicPrimitive>& data)
    {
        std::cout << "Добавляем один или несколько примитивов в документ" << std::endl;
        std::copy(data.begin(), data.end(), std::back_inserter(_data));
        return true;
    }

    /**
     * @brief Удалить графический примитив по индексу
     *
     * @param[in] index Индекс графического примитива
     * @return true графический примитив удалён
     * @return false графический примитив не удалён
     */
    bool remove(std::size_t index)
    {
        if (index >= _data.size())
        {
            std::cout << "Примитива с индексом " << index << " нет в документе" << std::endl;
            return false;
        }
        std::cout << "Удаляем примитив с индексом " << index << std::endl;
        _data.erase(_data.begin() + index);
        return true;
    }

    /**
     * @brief Получить графический примитив по индексу
     * 
     * @param index Индекс примитива
     * @param gp Ссылка на объект куда будет сохраняться данные примитива
     * @return true примитив получили
     * @return false примитив не получили
     */
    bool get(std::size_t index, IGraphicPrimitive& gp)
    {
        if (index >= _data.size())
        {
            std::cout << "Примитива с индексом " << index << " нет в документе" << std::endl;
            return false;
        }
        gp = _data[index];
        return true;
    }

    /**
     * @brief Получить массив графических примитивов
     *
     * @return Массив графических примитивов
     */
    std::vector<IGraphicPrimitive> getData()
    {
        return _data;
    }

    std::filesystem::path getPath() const
    {
        return _path;
    }

    void setPath(std::filesystem::path path)
    {
        _path = path;
    }

  private:
    /**
     * @brief Указатель на файловый менеджер
     *
     */
    std::shared_ptr<CFileManager> _fileManager;

    /**
     * @brief Путь к файлу документа
     *
     */
    std::filesystem::path _path;

    /**
     * @brief Данные документа
     *
     */
    std::vector<IGraphicPrimitive> _data;
};

#endif /* B26CA707_4BE2_49BC_A48F_D156D1030252 */
