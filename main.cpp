#include "triangle.h"
#include "vect.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

std::string user_input(const std::string& message) {
    std::cout << message;

    std::string input;
    if (!std::getline(std::cin, input)) {
        throw std::ios_base::failure("Ввод завершён");
    }
    return input;
}

int user_input_int(const std::string& message) {
    while (true) {
        std::istringstream stream(user_input(message));
        int value;

        if (stream >> value) {
            stream >> std::ws;
            if (stream.eof()) {
                return value;
            }
        }

        std::cout << "Ошибка: введите целое число.\n";
    }
}

int user_input_int_range(const std::string& message,
                         int min_value, int max_value) {
    while (true) {
        int value = user_input_int(message);

        if (value >= min_value && value <= max_value) {
            return value;
        }

        std::cout << "Ошибка: введите число от " << min_value
                  << " до " << max_value << ".\n";
    }
}

triangle user_input_triangle() {
    while (true) {
        try {
            std::string input = user_input(
                "Введите стороны A B C через пробел: "
            );
            return triangle(input);
        }
        catch (const std::ios_base::failure&) {
            throw;
        }
        catch (const std::exception& error) {
            std::cout << "Ошибка: " << error.what() << '\n';
        }
    }
}

void print_triangle(const triangle& figure, int index) {
    std::cout
        << "[" << index << "] "
        << "A = " << figure.get_side_a()
        << ", B = " << figure.get_side_b()
        << ", C = " << figure.get_side_c()
        << ", Площадь = " << figure.get_area()
        << '\n';
}

void print_vector(const vect<triangle>& triangles) {
    std::cout << "\nКоличество треугольников: "
              << triangles.get_size()
              << " / " << vect<triangle>::MAX_SIZE
              << '\n';

    if (triangles.is_empty()) {
        std::cout << "Вектор пуст.\n";
        return;
    }

    for (int i = 0; i < triangles.get_size(); ++i) {
        print_triangle(triangles[i], i);
    }
}

int main() {
    vect<triangle> triangles;

    try {
        while (true) {
            std::cout
                << "\nВектор треугольников:\n"
                << "1. Создать вектор треугольников\n"
                << "2. Вывести все треугольники\n"
                << "3. Получить треугольник по индексу\n"
                << "4. Добавить треугольник в конец\n"
                << "5. Вставить треугольник по индексу\n"
                << "6. Заменить треугольник\n"
                << "7. Удалить первый треугольник\n"
                << "8. Удалить последний треугольник\n"
                << "9. Очистить вектор\n"
                << "0. Выход\n";

            int choice = user_input_int_range("Выберите действие: ", 0, 9);

            if (choice == 0) {
                std::cout << "Завершение программы.\n";
                break;
            }

            try {
                switch (choice) {
                    case 1: {
                        int size = user_input_int_range(
                            "Количество треугольников (0..100): ",
                            0, vect<triangle>::MAX_SIZE
                        );

                        // Исходный вектор меняется только после полного ввода.
                        vect<triangle> new_triangles(size);

                        for (int i = 0; i < size; ++i) {
                            std::cout << "\nТреугольник " << i + 1 << ":\n";
                            new_triangles[i] = user_input_triangle();
                        }

                        triangles = std::move(new_triangles);
                        print_vector(triangles);
                        break;
                    }

                    case 2:
                        print_vector(triangles);
                        break;

                    case 3: {
                        if (triangles.is_empty()) {
                            std::cout << "Вектор пуст: нечего выводить.\n";
                            break;
                        }

                        int index = user_input_int_range(
                            "Введите индекс: ", 0, triangles.get_size() - 1
                        );
                        print_triangle(triangles[index], index);
                        break;
                    }

                    case 4: {
                        if (triangles.get_size() == vect<triangle>::MAX_SIZE) {
                            std::cout << "Вектор заполнен: добавить элемент нельзя.\n";
                            break;
                        }

                        triangle figure = user_input_triangle();
                        triangles.push_back(figure);
                        print_vector(triangles);
                        break;
                    }

                    case 5: {
                        if (triangles.get_size() == vect<triangle>::MAX_SIZE) {
                            std::cout << "Вектор заполнен: вставка невозможна.\n";
                            break;
                        }

                        int index = user_input_int_range(
                            "Введите позицию вставки: ",
                            0, triangles.get_size()
                        );

                        triangle figure = user_input_triangle();
                        triangles.insert(index, figure);
                        print_vector(triangles);
                        break;
                    }

                    case 6: {
                        if (triangles.is_empty()) {
                            std::cout << "Вектор пуст: нечего заменять.\n";
                            break;
                        }

                        int index = user_input_int_range(
                            "Введите индекс замены: ",
                            0, triangles.get_size() - 1
                        );

                        triangles[index] = user_input_triangle();
                        print_vector(triangles);
                        break;
                    }

                    case 7:
                        triangles.pop_front();
                        print_vector(triangles);
                        break;

                    case 8:
                        triangles.pop_back();
                        print_vector(triangles);
                        break;

                    case 9:
                        triangles.clear();
                        print_vector(triangles);
                        break;
                }
            }
            catch (const std::ios_base::failure&) {
                throw;
            }
            catch (const std::exception& error) {
                // Остальные ошибки операций не завершают работу программы.
                std::cout << "Ошибка: " << error.what() << '\n';
            }
        }
    }
    catch (const std::ios_base::failure&) {
        std::cout << "\nВвод завершён.\n";
    }
    catch (const std::exception& error) {
        std::cerr << "Критическая ошибка: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
