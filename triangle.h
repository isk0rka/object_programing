/* 2. Запрограммировать класс «Треугольник».

Определить в нем конструкторы и деструктор, перегрузить операцию преобразования в вещественное число 
(площадь треугольника), операцию преобразования в символьную строку и
 метод получения объекта-треугольника из строки.

Предусмотреть генерацию исключительных ситуаций:
1) попытка создать треугольник с нулевой или отрицательного значения стороной;
2) попытка создать несуществующий треугольник. */

#pragma once

#include <string>

class triangle
{
private:

    double side_a;
    double side_b;
    double side_c;

    bool valided_side(double side);

    bool is_valid_figure(double side_a, double side_b, double side_c);
    bool is_valid_sides(double side_a, double side_b, double side_c);
    bool valid_input_str(std::string input_data);

public:

    triangle();
    triangle(double input_side_a, double input_side_b, double input_side_c);
    triangle(std::string input_data);

    void set_side_a(double side);
    void set_side_b(double side);
    void set_side_c(double side);

    double get_side_a() const;
    double get_side_b() const;
    double get_side_c() const;
    double get_area() const;
    std::string get_str() const;

    ~triangle();
};