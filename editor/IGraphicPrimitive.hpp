#ifndef B895D1D7_C15F_4227_B2FF_449793A16AB0
#define B895D1D7_C15F_4227_B2FF_449793A16AB0

#include <CPoint.hpp>
#include <iostream>

/**
 * @brief Интерфейс графического примитива
 *
 */
class IGraphicPrimitive
{
  public:
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

  private:
    /**
     * @brief Координаты
     *
     */
    CPoint _coordanates;
};

#endif /* B895D1D7_C15F_4227_B2FF_449793A16AB0 */
