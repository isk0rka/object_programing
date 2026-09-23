#include <stdexcept>
#include <cmath>
#include <pyramid.h>

pyramid::pyramid(double input_square_side, double input_height) {
    set_side(input_square_side);
    set_height(input_height);
}

void pyramid::set_side(double input_side) {
    if (!std::isfinite(input_side) || input_side <= 0) {
        throw std::invalid_argument("Side must be a positive finite number");
    }

    square_side = input_side;
}

void pyramid::set_height(double input_height) {
    if (!std::isfinite(input_height) || input_height <= 0) {
        throw std::invalid_argument("Height must be a positive finite number");
    }

    height = input_height;
}

double pyramid::get_side() const {
    return square_side;
}

double pyramid::get_height() const {
    return height;
}

double pyramid::get_base_area() const {
    return square_side * square_side;
}

double pyramid::get_volume() const {
    return get_base_area() * height / 3.0;
}