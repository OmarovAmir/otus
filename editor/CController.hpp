#ifndef B8412352_4C9D_4162_B8F0_3AB1C072EB3C
#define B8412352_4C9D_4162_B8F0_3AB1C072EB3C

#include <CDocument.hpp>
#include <CFileManager.hpp>
#include <map>
#include <vector>
#include <memory>

class CController
{
  private:
    std::map<uint64_t, std::shared_ptr<CDocument>> _documents;
    std::shared_ptr<CFileManager> _fileManager;

  public:
    explicit CController()
        : _documents{}, _fileManager{std::make_shared<CFileManager>()}
    {
    }
    
    ~CController()
    {
        std::vector<uint64_t> handlers;
        for (const auto& document : _documents)
        {
            handlers.push_back(document.first);
        }
        for (const auto& handler: handlers)
        {
            closeDocument(handler);
        }
    }
    
    bool openDocument(uint64_t& documentHandler, const std::string filePath = "")
    {
        documentHandler = _documents.empty() ? 1 : _documents.rbegin()->first + 1;
        std::cout << std::endl
                  << "Открытие " 
                  << (filePath.empty() ? "нового" : "существующего")
                  << " документа {handler = " << documentHandler << "}" << std::endl;
        auto document = std::make_shared<CDocument>(_fileManager, filePath);
        if (!document)
        {
            documentHandler = 0;
            return false;
        }
        _documents.emplace(documentHandler, document);
        return true;
    }
    
    bool closeDocument(const uint64_t& documentHandler)
    {
        std::cout << std::endl
                  << "Закрытие документа {handler = " << documentHandler << "}" << std::endl;
        _documents.erase(documentHandler);
        return true;
    }

    bool addGraphicPrimitive(const uint64_t& documentHandler, const std::string& gpName)
    {
        std::cout << std::endl
                << "Добавление графического примитива" << std::endl;
        _documents[documentHandler]->add({IGraphicPrimitive{gpName, 0.0, 0.0}});
        return true;
    }

    bool removeGraphicPrimitive(const uint64_t& documentHandler, const std::size_t gpIndex)
    {
        std::cout << std::endl
                << "Добавление графического примитива" << std::endl;
        _documents[documentHandler]->remove(gpIndex);
        return true;
    }
};

#endif /* B8412352_4C9D_4162_B8F0_3AB1C072EB3C */
