#include "faculty.h"

#include <stdexcept>

faculty::faculty()
    : faculty("Unknown", "Unknown")
{
}

faculty::faculty(const std::string& name, const std::string& specialty_name)
    : name(name), specialty_name(specialty_name)
{
    if (name.find_first_not_of(" \t\n\r\v\f") == std::string::npos ||
        specialty_name.find_first_not_of(" \t\n\r\v\f") == std::string::npos) {
        throw std::invalid_argument("Invalid faculty or specialty name");
    }
}

std::string faculty::get_faculty_name() const {
    return name;
}

std::string faculty::get_specialty_name() const {
    return specialty_name;
}

bool faculty::add_group(const std::string& group_name) {
    for (const group& value : groups) {
        if (value.get_group_name() == group_name) {
            return false;
        }
    }

    groups.emplace_back(group_name);
    return true;
}

bool faculty::remove_group(const std::string& group_name) {
    for (group& value : groups) {
        if (value.get_group_name() == group_name) {
            return value.set_delete();
        }
    }

    return false;
}

bool faculty::has_group(const std::string& group_name) const {
    for (const group& value : groups) {
        if (!value.is_deleted() && value.get_group_name() == group_name) {
            return true;
        }
    }

    return false;
}

bool faculty::add_student(const std::string& group_name, const student& value) {
    if (has_student(value.get_id())) {
        return false;
    }

    for (group& current_group : groups) {
        if (current_group.get_group_name() == group_name) {
            return current_group.add_student(value);
        }
    }

    return false;
}

bool faculty::remove_student(const std::string& id) {
    for (group& value : groups) {
        if (value.remove_student(id)) {
            return true;
        }
    }

    return false;
}

bool faculty::has_student(const std::string& id) const {
    for (const group& value : groups) {
        if (value.has_student(id)) {
            return true;
        }
    }

    return false;
}

bool faculty::set_student_average_score(const std::string& id, double average_score) {
    if (!student::is_valid_average_score(average_score)) {
        throw std::invalid_argument("Invalid average score");
    }

    for (group& value : groups) {
        if (value.set_student_average_score(id, average_score)) {
            return true;
        }
    }

    return false;
}

std::vector<std::string> faculty::get_groups_list() const {
    std::vector<std::string> result;

    for (const group& value : groups) {
        if (!value.is_deleted()) {
            result.push_back(value.get_group_name());
        }
    }

    return result;
}

std::vector<student> faculty::get_students_by_group(const std::string& group_name) const {
    for (const group& value : groups) {
        if (value.get_group_name() == group_name) {
            return value.get_list();
        }
    }

    return {};
}

std::vector<student> faculty::get_students() const {
    std::vector<student> result;

    for (const group& value : groups) {
        const auto students = value.get_list();
        result.insert(result.end(), students.begin(), students.end());
    }

    return result;
}

std::vector<student> faculty::get_students_by_score(const double average_threshold) const {
    if (!student::is_valid_average_score(average_threshold)) {
        throw std::invalid_argument("Invalid average threshold");
    }

    std::vector<student> result;

    for (const group& value : groups) {
        const auto students = value.get_students_by_score(average_threshold);
        result.insert(result.end(), students.begin(), students.end());
    }

    return result;
}

void faculty::clear_students_by_score(const double average_threshold) {
    if (!student::is_valid_average_score(average_threshold)) {
        throw std::invalid_argument("Invalid average threshold");
    }

    for (group& value : groups) {
        value.clear_students_by_score(average_threshold);
    }
}
