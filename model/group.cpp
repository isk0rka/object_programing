#include "group.h"

#include <algorithm>
#include <stdexcept>

group::group()
    : group("Unknown")
{
}

group::group(const std::string& name)
    : name(name)
{
    if (name.find_first_not_of(" \t\n\r\v\f") == std::string::npos) {
        throw std::invalid_argument("Invalid group name");
    }
}

std::string group::get_group_name() const {
    return name;
}

bool group::is_deleted() const {
    return deleted;
}

bool group::empty() const {
    return std::none_of(list.begin(), list.end(), [](const student& value) {
        return !value.is_deleted();
    });
}

bool group::set_delete() {
    if (deleted || !empty()) {
        return false;
    }

    deleted = true;
    return true;
}

bool group::add_student(const student& value) {
    if (deleted || value.is_deleted() || has_student(value.get_id())) {
        return false;
    }

    list.push_back(value);
    return true;
}

bool group::remove_student(const std::string& id) {
    if (deleted) {
        return false;
    }

    for (student& value : list) {
        if (value.get_id() == id && !value.is_deleted()) {
            value.set_delete();
            return true;
        }
    }

    return false;
}

bool group::has_student(const std::string& id) const {
    for (const student& value : list) {
        if (value.get_id() == id) {
            return true;
        }
    }

    return false;
}

bool group::set_student_average_score(const std::string& id, double average_score) {
    if (!student::is_valid_average_score(average_score)) {
        throw std::invalid_argument("Invalid average score");
    }

    if (deleted) {
        return false;
    }

    for (student& value : list) {
        if (value.get_id() == id && !value.is_deleted()) {
            value.set_average_score(average_score);
            return true;
        }
    }

    return false;
}

std::vector<student> group::get_list() const {
    std::vector<student> result;

    if (!deleted) {
        for (const student& value : list) {
            if (!value.is_deleted()) {
                result.push_back(value);
            }
        }
    }

    return result;
}

std::optional<student> group::get_student(const std::string& id) const {
    if (!deleted) {
        for (const student& value : list) {
            if (value.get_id() == id && !value.is_deleted()) {
                return value;
            }
        }
    }

    return std::nullopt;
}

std::vector<student> group::get_students_by_score(double average_threshold) const {
    if (!student::is_valid_average_score(average_threshold)) {
        throw std::invalid_argument("Invalid average threshold");
    }

    std::vector<student> result;

    if (!deleted) {
        for (const student& value : list) {
            if (!value.is_deleted() && value.get_average_score() > average_threshold) {
                result.push_back(value);
            }
        }
    }

    return result;
}

void group::clear_students_by_score(double average_threshold) {
    if (!student::is_valid_average_score(average_threshold)) {
        throw std::invalid_argument("Invalid average threshold");
    }

    if (!deleted) {
        for (student& value : list) {
            if (!value.is_deleted() && value.get_average_score() < average_threshold) {
                value.set_delete();
            }
        }
    }
}
