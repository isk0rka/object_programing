#pragma once

#include <vector>
#include <cstddef>

#include "pyramid.h"

class pyramids_manager {
private:
    std::vector<pyramid> pyramids;

public:
    void add_pyramid(double side, double height);

    std::vector<const pyramid*> get_max_base_area() const;
    std::vector<const pyramid*> get_max_volume() const;

    const pyramid* get_pyramid(std::size_t index) const;

    std::size_t get_count() const;
};