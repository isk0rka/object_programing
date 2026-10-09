/*3. Запрограммировать класс «Треугольник». Определить в нем конструкторы и деструктор, перегрузить операцию преобразования
 в вещественное число (площадь треугольника), операцию преобразования в символьную строку и
метод получения объекта-треугольника из строки.
Предусмотреть генерацию исключительных ситуаций:
1) попытка создать треугольник с нулевой или отрицательного значения стороной;
2) попытка создать несуществующий треугольник.*/
#pragma once

class triangle
{
private:

    double side_a;
    double side_b;
    double side_c;

    void set_side_a(double input_side);
    void set_side_b(double input_side);
    void set_side_c(double input_side);

    bool check_life(double max, double input_side, double input_next_side);
    bool valid_data(double input_side_a, double input_side_b, double input_side_c);

public:

    triangle();
    triangle(double input_side_a, double input_side_b, double input_side_c);

    double get_side_a() const;
    double get_side_b() const;
    double get_aria() const;

    ~triangle();
};