#ifndef B26CA707_4BE2_49BC_A48F_D156D1030252
#define B26CA707_4BE2_49BC_A48F_D156D1030252

#include <IGraphicPrimitive.hpp>
#include <CFileManager.hpp>
#include <vector>
#include <iostream>

class CDocument
{
public:

    explicit CDocument(CFileManager fileManager, std::filesystem::path path):
    _fileManager{fileManager}, _path{path}
    {}

    bool save()
    {
        std::cout << "Сохраняем документ" << std::endl;
    }

    bool add(const std::vector<IGraphicPrimitive>& data)
    {
        std::cout << "Добавляем один или несколько примитивов в документ" << std::endl;
        _data.assign(data.cbegin(), data.cend());
        return true;
    }

    bool remove(std::size_t index)
    {
        if(index >= _data.size())
        {
            std::cout << "Примитива с индексом " << index << " нет в документе" << std::endl;
            return false;
        }
        std::cout << "Удаляем примитив с индексом " << index << std::endl;
        _data.erase(_data.begin() + index);
        return true;
    }

    std::vector<IGraphicPrimitive> getData()
    {
        return _data;
    }

private:
    CFileManager _fileManager;
    std::filesystem::path _path;
    std::vector<IGraphicPrimitive> _data;
};


#endif /* B26CA707_4BE2_49BC_A48F_D156D1030252 */
