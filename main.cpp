#include <CDocument.hpp>
#include <CFileManager.hpp>
#include <CPoint.hpp>
#include <IGraphicPrimitive.hpp>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

void createDocument()
{
    std::cout << std::endl
              << "Создание нового документа" << std::endl;
    auto fileManager = std::make_shared<CFileManager>();
    auto document = std::make_shared<CDocument>(fileManager);
    std::cout << std::endl;
}

void openDocument()
{
    std::cout << std::endl
              << "Открытие существующего документа" << std::endl;
    auto fileManager = std::make_shared<CFileManager>();
    auto document = std::make_shared<CDocument>(fileManager, "filePath");
    std::cout << std::endl;
}

void addGraphicPrimitiveNew()
{
    std::cout << std::endl
              << "Добавление графического примитива в новый файл" << std::endl;
    auto fileManager = std::make_shared<CFileManager>();
    auto document = std::make_shared<CDocument>(fileManager);
    document->add({IGraphicPrimitive{0.0, 0.0}});
    document->setPath("filePath");
}

void addGraphicPrimitiveExist()
{
    std::cout << std::endl
              << "Добавление графического примитива в существующий файл" << std::endl;
    auto fileManager = std::make_shared<CFileManager>();
    auto document = std::make_shared<CDocument>(fileManager, "filePath");
    document->add({IGraphicPrimitive{0.0, 0.0}});
    document->setPath("filePath");
}

void removeGraphicPrimitive()
{
    std::cout << std::endl
              << "Удаление графического примитива" << std::endl;
    auto fileManager = std::make_shared<CFileManager>();
    auto document = std::make_shared<CDocument>(fileManager, "filePath");
    document->remove(5);
    document->setPath("filePath");
}

int main()
{
    createDocument();
    openDocument();
    addGraphicPrimitiveNew();
    addGraphicPrimitiveExist();
    removeGraphicPrimitive();
    return 0;
}