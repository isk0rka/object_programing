#include "pyramids_manager.h"

void pyramids_manager::add_pyramid(double side, double height) {
    pyramids.emplace_back(side, height);
}

const pyramid* pyramids_manager::get_max_base_area() const {
    if (pyramids.empty()) {
        return nullptr;
    }

    size_t max_index = 0;
    for (size_t i = 1; i < pyramids.size(); i++) {
        if (pyramids[i].get_base_area() > pyramids[max_index].get_base_area()) {
            max_index = i;
        }
    }

    return &pyramids[max_index];
}

const pyramid* pyramids_manager::get_max_volume() const {
    if (pyramids.empty()) {
        return nullptr;
    }

    size_t max_index = 0;

    for (size_t i = 1; i < pyramids.size(); i++) {
        if (pyramids[i].get_volume() > pyramids[max_index].get_volume()) {
            max_index = i;
        }
    }

    return &pyramids[max_index];
}

int pyramids_manager::get_count() const {
    return pyramids.size();
}