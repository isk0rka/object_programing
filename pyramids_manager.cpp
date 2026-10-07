#include "pyramids_manager.h"

#include <stdexcept>
#include <cmath>

void pyramids_manager::add_pyramid(double side, double height) {
    pyramids.emplace_back(side, height);
}

std::vector<const pyramid*> pyramids_manager::get_max_base_area() const {
    if (pyramids.empty()) {
        throw std::runtime_error(
            "Невозможно найти наибольшую площадь: список пирамид пуст."
        );
    }

    double max_area = pyramids[0].get_base_area();

    for (std::size_t i = 1; i < pyramids.size(); ++i) {
        if (pyramids[i].get_base_area() > max_area) {
            max_area = pyramids[i].get_base_area();
        }
    }

    std::vector<const pyramid*> result;

    for (const pyramid& object : pyramids) {
        if (std::abs(object.get_base_area() - max_area) < 1e-9) {
            result.push_back(&object);
        }
    }

    if (pyramids.size() > 1 &&
        result.size() == pyramids.size()) {

        throw std::logic_error(
            "Все пирамиды имеют одинаковую площадь основания."
        );
    }

    return result;
}

std::vector<const pyramid*> pyramids_manager::get_max_volume() const {
    if (pyramids.empty()) {
        throw std::runtime_error(
            "Невозможно найти наибольший объем: список пирамид пуст."
        );
    }

    double max_volume = pyramids[0].get_volume();

    for (std::size_t i = 1; i < pyramids.size(); ++i) {
        if (pyramids[i].get_volume() > max_volume) {
            max_volume = pyramids[i].get_volume();
        }
    }

    std::vector<const pyramid*> result;

    for (const pyramid& object : pyramids) {
        if (std::abs(object.get_volume() - max_volume) < 1e-9) {
            result.push_back(&object);
        }
    }

    if (pyramids.size() > 1 &&
        result.size() == pyramids.size()) {

        throw std::logic_error(
            "Все пирамиды имеют одинаковый объем."
        );
    }

    return result;
}

const pyramid* pyramids_manager::get_pyramid(std::size_t index) const {
    if (index >= pyramids.size()) {
        return nullptr;
    }

    return &pyramids[index];
}

std::size_t pyramids_manager::get_count() const {
    return pyramids.size();
}