#ifndef D5D3F35B_E90A_4BD5_A89A_44779F125157
#define D5D3F35B_E90A_4BD5_A89A_44779F125157

/**
 * @brief Класс хранящий координаты конкретной точки на экране
 *
 */
class CPoint
{
  public:
    /**
     * @brief Получить значение координаты X
     *
     * @return double Значение коордитаны X
     */
    double getX() { return _x; }

    /**
     * @brief Получить значение координаты Y
     *
     * @return double Значение коордитаны Y
     */
    double getY() { return _y; }

    /**
     * @brief Установить значение координаты X
     *
     * @param[in] x Значение координаты X
     */
    void setX(double x) { _x = x; }

    /**
     * @brief Установить значение координаты Y
     *
     * @param[in] y Значение координаты Y
     */
    void setY(double y) { _y = y; }

    /**
     * @brief Установить значение координат
     *
     * @param[in] x Координата X
     * @param[in] y Координата Y
     */
    void set(double x, double y)
    {
        _x = x;
        _y = y;
    }

  private:
    /**
     * @brief Координата X
     *
     */
    double _x;

    /**
     * @brief Координата Y
     *
     */
    double _y;
};

#endif /* D5D3F35B_E90A_4BD5_A89A_44779F125157 */
