#pragma once

#include <vector>
#include <cstddef>

#include "pyramid.h"

class pyramids_manager {
private:
    std::vector<pyramid> pyramids;

public:
    void add_pyramid(double side, double height);

    const pyramid* get_max_base_area() const;
    const pyramid* get_max_volume() const;

    const pyramid* get_pyramid(std::size_t index) const;
    
    std::size_t get_count() const;
};