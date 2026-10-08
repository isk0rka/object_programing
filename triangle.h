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

    void set_side_a(double input_side);
    void set_side_b(double input_side);

public:

    triangle();
    triangle(double input_side);
    triangle(double input_side_a, double input_side_b);

    double get_side_a() const;
    double get_side_b() const;
    double get_aria() const;

    ~triangle();
};