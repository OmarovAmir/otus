#include <iostream>
#include <matrix.hpp>
#include <bitset>

int main()
{
    {
        std::bitset<sizeof(std::size_t) * 8 * 2> x;
        std::array<std::size_t, 2> a;
        a[0] = 1000000;
        a[1] = 1000000000;
        for (const auto& i: a)
        {
            x <<= sizeof(i) * 8;
            x |= i;
        }
        std::hash<decltype(x)> hashBits;
        std::cout << x << std::endl;
        std::cout << hashBits(x) << std::endl;
    }

    // {
    //     std::cout << "Пример из задания" << std::endl;
    //     matrix<int, -1> matrix;
    //     assert(matrix.size() == 0); // все ячейки свободны

    //     auto a = matrix[0][0];
    //     assert(a == -1);
    //     assert(matrix.size() == 0);

    //     matrix[100][100] = 314;
    //     assert(matrix[100][100] == 314);
    //     assert(matrix.size() == 1);

    //     // выведется одна строка
    //     // 100100314
    //     for (const auto c : matrix)
    //     {
    //         int x;
    //         int y;
    //         int v;
    //         std::tie(x, y, v) = c;
    //         std::cout << x << y << v << std::endl;
    //     }
    // }

    // {
    //     std::cout << std::endl;
    //     // Конструктор по умолчанию
    //     matrix<int, -1> matrix1;
    //     assert(matrix1.size() == 0);
    //     matrix1[0][0] = 0;
    //     assert(matrix1.size() == 1);

    //     // Конструктор копирования
    //     auto matrix2 = matrix1;
    //     assert(matrix1.size() == 1);
    //     assert(matrix2.size() == 1);
    //     matrix1[1][1] = 11;
    //     assert(matrix1.size() == 2);
    //     assert(matrix2.size() == 1);

    //     // Конструктор перемещения
    //     auto matrix3 = std::move(matrix2);
    //     assert(matrix2.size() == 0);
    //     assert(matrix3.size() == 1);
    //     matrix2[1][1] = 11;
    //     assert(matrix2.size() == 1);
    //     assert(matrix3.size() == 1);

    //     // Конструктор копирования
    //     auto matrix4{matrix3};
    //     assert(matrix3.size() == 1);
    //     assert(matrix4.size() == 1);
    //     matrix3[1][1] = 11;
    //     assert(matrix3.size() == 2);
    //     assert(matrix4.size() == 1);

    //     // Конструктор перемещения
    //     auto matrix5{std::move(matrix4)};
    //     assert(matrix4.size() == 0);
    //     assert(matrix5.size() == 1);
    //     matrix4[1][1] = 11;
    //     assert(matrix4.size() == 1);
    //     assert(matrix5.size() == 1);

    //     // Копирующий оператор присваивания
    //     matrix1[2][2] = 11;
    //     assert(matrix1.size() == 3);
    //     assert(matrix5.size() == 1);
    //     matrix5 = matrix1;
    //     assert(matrix1.size() == 3);
    //     assert(matrix5.size() == 3);

    //     // Перемещающий оператор присваивания
    //     matrix1[3][3] = 11;
    //     assert(matrix1.size() == 4);
    //     assert(matrix5.size() == 3);
    //     matrix5 = std::move(matrix1);
    //     assert(matrix1.size() == 0);
    //     assert(matrix5.size() == 4);

    //     // Каноничная форма оператора присваивания
    //     assert((((matrix1[9][9] = 99) = 88) = 77) == 77);
    //     (((matrix1[0][0] = 0)[1][1] = 11)[2][2] = 22)[3][3] = 33;
    //     assert(matrix1[0][0] == 0);
    //     assert(matrix1[1][1] == 11);
    //     assert(matrix1[2][2] == 22);
    //     assert(matrix1[3][3] == 33);
    // }

    // {
    //     std::cout << "Задание" << std::endl;
    //     matrix<int, 0> matrix;
    //     for (int i = 0; i < 10; ++i)
    //     {
    //         matrix[i][i] = i;
    //         matrix[i][9 - i] = 9 - i;
    //     }
    //     for (int i = 1; i < 9; ++i)
    //     {
    //         for (int j = 1; j < 9; ++j)
    //         {
    //             std::cout << *matrix[i][j] << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    //     std::cout << "" << matrix.size() << std::endl;
    //     for (const auto& c : matrix)
    //     {
    //         int x;
    //         int y;
    //         int v;
    //         std::tie(x, y, v) = c;
    //         std::cout << "[" << x << "][" << y << "] = " << v << std::endl;
    //     }
    // }

    return 0;
}
