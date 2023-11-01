#ifndef B895D1D7_C15F_4227_B2FF_449793A16AB0
#define B895D1D7_C15F_4227_B2FF_449793A16AB0

#include <CPoint.hpp>
#include <iostream>
#include <string>

/**
 * @brief Интерфейс графического примитива
 *
 */
class IGraphicPrimitive
{
  public:
    explicit IGraphicPrimitive(std::string name, double x, double y)
        : _name{name}
    {
        _coordanates.set(x, y);
    }

    /**
     * @brief Метод отрисовки
     *
     */
    virtual void draw()
    {
        std::cout << "Отрисовка графического примитива с начальными координатами {"
                  << "x = " << _coordanates.getX() << ", "
                  << "y = " << _coordanates.getY() << "}"
                  << std::endl;
    };

    virtual std::string exportData() const
    {
        return "Данные примитива { " + getName() + "; " + std::to_string(_coordanates.getX()) + "; " + std::to_string(_coordanates.getY()) + " }";
    }

    virtual bool importData([[maybe_unused]] std::string dataString)
    {
        std::cout << dataString << std::endl;
        return true;
    }

    /**
     * @brief Установка координат
     *
     * @param[in] x Значение координаты X
     * @param[in] y Значение координаты Y
     */
    void setCoordinates(double x, double y)
    {
        _coordanates.set(x, y);
    }

    /**
     * @brief Получить координаты
     *
     * @return Координаты
     */
    CPoint getCoordinates() const
    {
        return _coordanates;
    }

    /**
     * @brief Получить координаты
     *
     * @return Координаты
     */
    std::string getName() const
    {
        return _name;
    }

  private:
    /**
     * @brief Координаты
     *
     */
    CPoint _coordanates;
    std::string _name;
};

#endif /* B895D1D7_C15F_4227_B2FF_449793A16AB0 */
