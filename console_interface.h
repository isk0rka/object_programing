#pragma once

#include "pyramids_manager.h"

class console_interface {
private:
    pyramids_manager manager;

    int input_positive_int();

    bool valid_input_pyramids(double side, double height);

    void input_pyramids();

    void print_pyramid(const pyramid& object);
    void print_all_pyramids();

public:
    void run();
};