#include "triangle.h"

#include <sstream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <algorithm>

bool triangle::valided_side(double side){
    if (!std::isfinite(side) || side <= 0) {
        throw std::invalid_argument("сторона должна быть положительным конечным числом.");
    }
    return true;
}


bool triangle::is_valid_sides(double side_a, double side_b, double side_c){
    if (valided_side(side_a) && valided_side(side_b) && valided_side(side_c)){
        return true;
    }
    return false;
}


bool triangle::is_valid_figure(double side_a, double side_b, double side_c){
    if (!((2 * std::max({side_a,side_b,side_c})) < (side_a + side_b + side_c))) {
        throw std::invalid_argument( "Треугольник с такими сторонами не существует.");
    }
    return true; 
}

bool triangle::valid_input_str(std::string input_data_str){
    if (input_data_str.empty()){
        return false;
    }

    std::istringstream stream(input_data_str);

    double input_side_a;
    double input_side_b;
    double input_side_c;

    if (!(stream >> input_side_a >> input_side_b >> input_side_c)){
        return false;
    }

    stream >> std::ws;

    return stream.eof();
}


triangle::triangle(){
    side_a = 1.0;
    side_b = 1.0;
    side_c = 1.0;
}

triangle::triangle(double input_side_a, double input_side_b, double input_side_c){
    if ((is_valid_sides(input_side_a, input_side_b, input_side_c)) 
        && (is_valid_figure(input_side_a, input_side_b, input_side_c))){

        side_a = input_side_a;
        side_b = input_side_b;
        side_c = input_side_c;
    }           
}


triangle::triangle(std::string input_data_str): triangle([&]() {
        if (!valid_input_str(input_data_str)){
            throw std::invalid_argument("Некорректный формат строки.");
        }

        std::istringstream stream(input_data_str);

        double input_side_a;
        double input_side_b;
        double input_side_c;

        stream >> input_side_a >> input_side_b >> input_side_c;

        return triangle( input_side_a, input_side_b, input_side_c);
    }())
{
}

void triangle::set_side_a(double side){
    if ((valided_side(side)) 
        && (is_valid_figure(side, side_b, side_c))){
        side_a = side;
    }
}

void triangle::set_side_b(double side){
    if ((valided_side(side)) 
        && (is_valid_figure(side, side_b, side_c))){
        side_b = side;
    }
}

void triangle::set_side_c(double side){
    if ((valided_side(side)) 
        && (is_valid_figure(side, side_b, side_c))){
        side_c = side;
    }
}


double triangle::get_side_a() const{
    return side_a;
}

double triangle::get_side_b() const{
    return side_b;
}


double triangle::get_side_c() const{
    return side_c;
}


double triangle::get_area() const{
    double p = (side_a + side_b + side_c) / 2.0;
    return std::sqrt( p * (p - side_a) * (p - side_b) * (p - side_c));
}


std::string triangle::get_str() const{
    return std::to_string(side_a) + " " + std::to_string(side_b) + " " + std::to_string(side_c);
}


triangle::~triangle(){
}