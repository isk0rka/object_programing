#include "triangle.h"

#include <stdexcept>
#include <algorithm>
#include <cmath>

void triangle::set_side_a(double input_side){
    if (!std::isfinite(input_side) || input_side <= 0) {
        throw std::invalid_argument("сторона A должна быть положительным конечным числом.");
    }
    side_a = input_side;
}

void triangle::set_side_b(double input_side){
    if (!std::isfinite(input_side) || input_side <= 0) {
        throw std::invalid_argument("сторона B быть положительным конечным числом.");
    }
    side_b = input_side;
}

void triangle::set_side_c(double input_side){
    if (!std::isfinite(input_side) || input_side <= 0) {
        throw std::invalid_argument("сторона C быть положительным конечным числом.");
    }
    side_b = input_side;
}


bool triangle::check_life(double max, double input_side, double input_next_side){
    if (max < (input_side + input_next_side)){
        return true;
    } else {
        return false;
    }
}


bool triangle::valid_data(double input_side_a, double input_side_b, double input_side_c){

    double max = std::max(input_side_a, input_side_b, input_side_c);
    
    if (max == input_side_a) {
        return check_life(max, input_side_b, input_side_c);
    } else if (max == input_side_b) {
        return check_life(max, input_side_a, input_side_c);
    } else if (max == input_side_c) {
        return check_life(max, input_side_a, input_side_b);
    } else {
    }
    
}


triangle::triangle(){
    side_a = 1.0;
    side_b = 1.0;
    side_c = 1.0;
}

triangle::triangle(double input_side_a, double input_side_b, double input_side_c){
    set_side_a(input_side_a);
    set_side_b(input_side_b);
    set_side_c(input_side_c);
}


double triangle::get_side_a() const{
    return side_a;
}

double triangle::get_side_b() const{
    return side_b;
}

double triangle::get_aria() const{
    if ((side_a >= 0.0) && (side_b >= 0.0)){
        return((side_a * side_b) / 2);
    } else {
        return ((side_a * side_a) / 2);
    }
}