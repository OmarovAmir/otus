#include <chrono>
#include <random>
#include <iostream>
#include <matrix.hpp>
#include <unordered_matrix.hpp>


int main()
{
    {
        // Конструктор по умолчанию
        matrix<int, -1> matrix1;
        assert(matrix1.size() == 0);
        matrix1[0][0] = 0;
        assert(matrix1.size() == 1);

        // Конструктор копирования
        auto matrix2 = matrix1;
        assert(matrix1.size() == 1);
        assert(matrix2.size() == 1);
        matrix1[1][1] = 11;
        assert(matrix1.size() == 2);
        assert(matrix2.size() == 1);

        // Конструктор перемещения
        auto matrix3 = std::move(matrix2);
        assert(matrix2.size() == 0);
        assert(matrix3.size() == 1);
        matrix2[1][1] = 11;
        assert(matrix2.size() == 1);
        assert(matrix3.size() == 1);

        // Конструктор копирования
        auto matrix4{matrix3};
        assert(matrix3.size() == 1);
        assert(matrix4.size() == 1);
        matrix3[1][1] = 11;
        assert(matrix3.size() == 2);
        assert(matrix4.size() == 1);

        // Конструктор перемещения
        auto matrix5{std::move(matrix4)};
        assert(matrix4.size() == 0);
        assert(matrix5.size() == 1);
        matrix4[1][1] = 11;
        assert(matrix4.size() == 1);
        assert(matrix5.size() == 1);

        // Копирующий оператор присваивания
        matrix1[2][2] = 11;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 1);
        matrix5 = matrix1;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 3);

        // Перемещающий оператор присваивания
        matrix1[3][3] = 11;
        assert(matrix1.size() == 4);
        assert(matrix5.size() == 3);
        matrix5 = std::move(matrix1);
        assert(matrix1.size() == 0);
        assert(matrix5.size() == 4);

        // Каноничная форма оператора присваивания
        assert((((matrix1[9][9] = 99) = 88) = 77) == 77);
        (((matrix1[0][0] = 0)[1][1] = 11)[2][2] = 22)[3][3] = 33;
        assert(matrix1[0][0] == 0);
        assert(matrix1[1][1] == 11);
        assert(matrix1[2][2] == 22);
        assert(matrix1[3][3] == 33);
    }

    {
        // Конструктор по умолчанию
        unordered_matrix<int, -1> matrix1;
        assert(matrix1.size() == 0);
        matrix1[0][0] = 0;
        assert(matrix1.size() == 1);

        // Конструктор копирования
        auto matrix2 = matrix1;
        assert(matrix1.size() == 1);
        assert(matrix2.size() == 1);
        matrix1[1][1] = 11;
        assert(matrix1.size() == 2);
        assert(matrix2.size() == 1);

        // Конструктор перемещения
        auto matrix3 = std::move(matrix2);
        assert(matrix2.size() == 0);
        assert(matrix3.size() == 1);
        matrix2[1][1] = 11;
        assert(matrix2.size() == 1);
        assert(matrix3.size() == 1);

        // Конструктор копирования
        auto matrix4{matrix3};
        assert(matrix3.size() == 1);
        assert(matrix4.size() == 1);
        matrix3[1][1] = 11;
        assert(matrix3.size() == 2);
        assert(matrix4.size() == 1);

        // Конструктор перемещения
        auto matrix5{std::move(matrix4)};
        assert(matrix4.size() == 0);
        assert(matrix5.size() == 1);
        matrix4[1][1] = 11;
        assert(matrix4.size() == 1);
        assert(matrix5.size() == 1);

        // Копирующий оператор присваивания
        matrix1[2][2] = 11;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 1);
        matrix5 = matrix1;
        assert(matrix1.size() == 3);
        assert(matrix5.size() == 3);

        // Перемещающий оператор присваивания
        matrix1[3][3] = 11;
        assert(matrix1.size() == 4);
        assert(matrix5.size() == 3);
        matrix5 = std::move(matrix1);
        assert(matrix1.size() == 0);
        assert(matrix5.size() == 4);

        // Каноничная форма оператора присваивания
        assert((((matrix1[9][9] = 99) = 88) = 77) == 77);
        (((matrix1[0][0] = 0)[1][1] = 11)[2][2] = 22)[3][3] = 33;
        assert(matrix1[0][0] == 0);
        assert(matrix1[1][1] == 11);
        assert(matrix1[2][2] == 22);
        assert(matrix1[3][3] == 33);
    }

    {
        std::cout << "Пример из задания (matrix)" << std::endl;
        matrix<int, -1> matrix;
        assert(matrix.size() == 0); // все ячейки свободны

        auto a = matrix[0][0];
        assert(a == -1);
        assert(matrix.size() == 0);

        matrix[100][100] = 314;
        assert(matrix[100][100] == 314);
        assert(matrix.size() == 1);

        // выведется одна строка
        // 100100314
        for (const auto c : matrix)
        {
            int x;
            int y;
            int v;
            std::tie(x, y, v) = c;
            std::cout << x << y << v << std::endl;
        }
        std::cout << std::endl;
    }

    {
        std::cout << "Пример из задания (unordered_matrix)" << std::endl;
        unordered_matrix<int, -1> matrix;
        assert(matrix.size() == 0); // все ячейки свободны

        auto a = matrix[0][0];
        assert(a == -1);
        assert(matrix.size() == 0);

        matrix[100][100] = 314;
        assert(matrix[100][100] == 314);
        assert(matrix.size() == 1);

        // выведется одна строка
        // 100100314
        for (const auto c : matrix)
        {
            int x;
            int y;
            int v;
            std::tie(x, y, v) = c;
            std::cout << x << y << v << std::endl;
        }
        std::cout << std::endl;
    }

    {
        std::cout << "Задание (matrix)" << std::endl;
        matrix<int, 0> matrix;
        for (int i = 0; i < 10; ++i)
        {
            matrix[i][i] = i;
            matrix[i][9 - i] = 9 - i;
        }
        for (int i = 1; i < 9; ++i)
        {
            for (int j = 1; j < 9; ++j)
            {
                std::cout << *matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << matrix.size() << std::endl;
        for (const auto& c : matrix)
        {
            int x;
            int y;
            int v;
            std::tie(x, y, v) = c;
            std::cout << "[" << x << "][" << y << "] = " << v << std::endl;
        }
        std::cout << std::endl;
    }

    {
        std::cout << "Задание (unordered_matrix)" << std::endl;
        unordered_matrix<int, 0> matrix;
        for (int i = 0; i < 10; ++i)
        {
            matrix[i][i] = i;
            matrix[i][9 - i] = 9 - i;
        }
        for (int i = 1; i < 9; ++i)
        {
            for (int j = 1; j < 9; ++j)
            {
                std::cout << *matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << matrix.size() << std::endl;
        for (const auto& c : matrix)
        {
            int x;
            int y;
            int v;
            std::tie(x, y, v) = c;
            std::cout << "[" << x << "][" << y << "] = " << v << std::endl;
        }
        std::cout << std::endl;
    }

    std::chrono::milliseconds elapsed_ms_matrix_add;
    std::chrono::milliseconds elapsed_ms_unordered_matrix_add;
    std::chrono::milliseconds elapsed_ms_matrix_get;
    std::chrono::milliseconds elapsed_ms_unordered_matrix_get;
    std::chrono::milliseconds elapsed_ms_matrix_clear;
    std::chrono::milliseconds elapsed_ms_unordered_matrix_clear;

    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::system_clock::now().time_since_epoch()
                ).count() +
            (std::mt19937::result_type)
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now().time_since_epoch()
                ).count() );

    std::mt19937 gen(seed);
    std::uniform_int_distribution<std::size_t> distrib(0, SIZE_MAX);
    std::size_t count = 10000000;

    {
        std::cout << "Создание матрицы из " << count << " элементов (matrix)" << std::endl;
        auto begin = std::chrono::steady_clock::now();
        matrix<std::size_t, 0> matrix;
        for (std::size_t i = 1; i <= count; ++i)
        {
            matrix[distrib(gen)][distrib(gen)] = i;
        }
        auto end = std::chrono::steady_clock::now();
        elapsed_ms_matrix_add = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        begin = std::chrono::steady_clock::now();
        std::vector<std::size_t> s;
        s.reserve(count);
        for (const auto& c : matrix)
        {
            std::size_t x;
            std::size_t y;
            std::size_t v;
            std::tie(x, y, v) = c;
            s.emplace_back(v);
        }
        end = std::chrono::steady_clock::now();
        elapsed_ms_matrix_get = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        begin = std::chrono::steady_clock::now();
        matrix.clear();
        end = std::chrono::steady_clock::now();
        elapsed_ms_matrix_clear = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    }

    {
        std::cout << "Создание матрицы из " << count << " элементов (unordered_matrix)" << std::endl;
        auto begin = std::chrono::steady_clock::now();
        unordered_matrix<std::size_t, 0> matrix;
        for (std::size_t i = 1; i <= count; ++i)
        {
            matrix[distrib(gen)][distrib(gen)] = i;
        }
        auto end = std::chrono::steady_clock::now();
        elapsed_ms_unordered_matrix_add = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        begin = std::chrono::steady_clock::now();
        std::vector<std::size_t> s;
        s.reserve(count);
        for (const auto& c : matrix)
        {
            std::size_t x;
            std::size_t y;
            std::size_t v;
            std::tie(x, y, v) = c;
            s.emplace_back(v);
        }
        end = std::chrono::steady_clock::now();
        elapsed_ms_unordered_matrix_get = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
        begin = std::chrono::steady_clock::now();
        matrix.clear();
        end = std::chrono::steady_clock::now();
        elapsed_ms_unordered_matrix_clear = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    }
    std::cout << "Время создания (matrix): " << elapsed_ms_matrix_add.count() << " ms\n";
    std::cout << "Время создания (unordered_matrix): " << elapsed_ms_unordered_matrix_add.count() << " ms\n";
    std::cout << "Время обращения (matrix): " << elapsed_ms_matrix_get.count() << " ms\n";
    std::cout << "Время обращения (unordered_matrix): " << elapsed_ms_unordered_matrix_get.count() << " ms\n";
    std::cout << "Время удаления (matrix): " << elapsed_ms_matrix_clear.count() << " ms\n";
    std::cout << "Время удаления (unordered_matrix): " << elapsed_ms_unordered_matrix_clear.count() << " ms\n";

    return 0;
}
