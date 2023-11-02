#ifndef B8412352_4C9D_4162_B8F0_3AB1C072EB3C
#define B8412352_4C9D_4162_B8F0_3AB1C072EB3C

#include <CDocument.hpp>
#include <CFileManager.hpp>
#include <functional>
#include <map>
#include <memory>
#include <vector>

class CController
{
  public:
    /**
     * @brief Конструктор
     * 
     */
    explicit CController()
        : _documents{}, _fileManager{std::make_shared<CFileManager>()}
    {
    }

    /**
     * @brief Деструктор
     * 
     */
    ~CController()
    {
        std::vector<uint64_t> handlers;
        for (const auto& document : _documents)
        {
            handlers.push_back(document.first);
        }
        for (const auto& handler : handlers)
        {
            closeDocument(handler);
        }
    }

    /**
     * @brief Открыть документ
     * 
     * @param[out] documentHandler Хендлер документа
     * @param[in] filePath Путь к документу
     * @return true Документ открыт
     * @return false Документ не открыт
     */
    bool openDocument(uint64_t& documentHandler, const std::string filePath = "")
    {
        bool result = false;
        documentHandler = _documents.empty() ? 1 : _documents.rbegin()->first + 1;
        std::cout << std::endl
                  << "Открытие "
                  << (filePath.empty() ? "нового" : "существующего")
                  << " документа {handler = " << documentHandler << "}" << std::endl;
        auto document = std::make_shared<CDocument>(_fileManager, filePath);
        if (document)
        {
            result = _documents.emplace(documentHandler, document).second;
            drawDocument(documentHandler);
        }
        else
        {
            documentHandler = 0;
            std::cout << "Не удалось открыть документ" << std::endl;
        }
        return result;
    }

    /**
     * @brief Закрыть документ
     * 
     * @param documentHandler Хендлер документа
     * @return true Документ закрыт
     * @return false Документ не закрыт
     */
    bool closeDocument(const uint64_t& documentHandler)
    {
        std::cout << std::endl
                  << "Закрытие документа" << std::endl;
        bool result = false;
        if (_documents.find(documentHandler) != _documents.end())
        {
            result = (1 == _documents.erase(documentHandler));
        }
        else
        {
            std::cout << std::endl
                      << "Документ отсутствует {handler = " << documentHandler << "}" << std::endl;
        }
        return result;
    }

    /**
     * @brief Добавить графический примитив
     * 
     * @param documentHandler Хендлер документа
     * @param gpName Имя графического примитива
     * @return true Примитив добавлен
     * @return false Примитив не добавлен
     */
    bool addGraphicPrimitive(const uint64_t& documentHandler, const std::string& gpName)
    {
        std::cout << std::endl
                  << "Добавление графического примитива" << std::endl;
        bool result = false;
        if (_documents.find(documentHandler) != _documents.end())
        {
            result = _documents[documentHandler]->add({IGraphicPrimitive{gpName, 0.0, 0.0}});
            drawDocument(documentHandler);
        }
        else
        {
            std::cout << std::endl
                      << "Документ отсутствует {handler = " << documentHandler << "}" << std::endl;
        }
        return result;
    }

    /**
     * @brief Удалить графический примитив
     * 
     * @param documentHandler Хердлер документа
     * @param gpIndex Индекс графического примитива
     * @return true Примитив удалён
     * @return false Примитив не удалён
     */
    bool removeGraphicPrimitive(const uint64_t& documentHandler, const std::size_t gpIndex)
    {
        std::cout << std::endl
                  << "Удаление графического примитива" << std::endl;
        bool result = false;
        if (_documents.find(documentHandler) != _documents.end())
        {
            result = _documents[documentHandler]->remove(gpIndex);
            drawDocument(documentHandler);
        }
        else
        {
            std::cout << std::endl
                      << "Документ отсутствует {handler = " << documentHandler << "}" << std::endl;
        }
        return result;
    }

  private:
    /**
     * @brief Документы
     * 
     */
    std::map<uint64_t, std::shared_ptr<CDocument>> _documents;

    /**
     * @brief Файловый менеджер
     * 
     */
    std::shared_ptr<CFileManager> _fileManager;

    /**
     * @brief Отрисовать содержимое документа
     * 
     * @param document 
     */
    void drawDocument(const uint64_t& documentHandler)
    {
        if (!_documents[documentHandler]->empty())
        {
            _documents[documentHandler]->draw();
        }
    }
};

#endif /* B8412352_4C9D_4162_B8F0_3AB1C072EB3C */
