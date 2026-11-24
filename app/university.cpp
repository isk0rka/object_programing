#include "university.h"

#include <stdexcept>

faculty* university::find_faculty(const std::string& name, const std::string& specialty_name) {
    for (faculty& value : faculties) {
        if (value.get_faculty_name() == name && value.get_specialty_name() == specialty_name) {
            return &value;
        }
    }

    return nullptr;
}

bool university::add_faculty(const std::string& name, const std::string& specialty_name) {
    if (find_faculty(name, specialty_name) != nullptr) {
        return false;
    }

    faculties.emplace_back(name, specialty_name);
    return true;
}

bool university::add_group(const std::string& name, const std::string& specialty_name,
                           const std::string& group_name) {
    faculty* value = find_faculty(name, specialty_name);
    return value != nullptr && value->add_group(group_name);
}

bool university::remove_group(const std::string& name, const std::string& specialty_name,
                              const std::string& group_name) {
    faculty* value = find_faculty(name, specialty_name);
    return value != nullptr && value->remove_group(group_name);
}

bool university::add_student(const std::string& name, const std::string& specialty_name,
                             const std::string& group_name, const student& value) {
    faculty* current_faculty = find_faculty(name, specialty_name);
    return current_faculty != nullptr && !has_student(value.get_id()) &&
           current_faculty->add_student(group_name, value);
}

bool university::remove_student(const std::string& id) {
    for (faculty& value : faculties) {
        if (value.remove_student(id)) {
            return true;
        }
    }

    return false;
}

bool university::has_student(const std::string& id) const {
    for (const faculty& value : faculties) {
        if (value.has_student(id)) {
            return true;
        }
    }

    return false;
}

bool university::set_student_average_score(const std::string& id, double average_score) {
    if (!student::is_valid_average_score(average_score)) {
        throw std::invalid_argument("Invalid average score");
    }

    for (faculty& value : faculties) {
        if (value.set_student_average_score(id, average_score)) {
            return true;
        }
    }

    return false;
}

const std::vector<faculty>& university::get_faculties() const {
    return faculties;
}

std::vector<student> university::get_students_by_faculty(const std::string& name) const {
    std::vector<student> result;

    for (const faculty& value : faculties) {
        if (value.get_faculty_name() == name) {
            const auto students = value.get_students();
            result.insert(result.end(), students.begin(), students.end());
        }
    }

    return result;
}

std::vector<student> university::get_students_by_score(double average_threshold) const {
    if (!student::is_valid_average_score(average_threshold)) {
        throw std::invalid_argument("Invalid average threshold");
    }

    std::vector<student> result;

    for (const faculty& value : faculties) {
        const auto students = value.get_students_by_score(average_threshold);
        result.insert(result.end(), students.begin(), students.end());
    }

    return result;
}

bool university::clear_students_by_score(const std::string& name, const std::string& specialty_name,
                                         double average_threshold) {
    if (!student::is_valid_average_score(average_threshold)) {
        throw std::invalid_argument("Invalid average threshold");
    }

    faculty* value = find_faculty(name, specialty_name);
    if (value == nullptr) {
        return false;
    }

    value->clear_students_by_score(average_threshold);
    return true;
}
