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

const faculty* university::find_faculty(const std::string& name, const std::string& specialty_name) const {
    for (const faculty& value : faculties) {
        if (value.get_faculty_name() == name && value.get_specialty_name() == specialty_name) {
            return &value;
        }
    }

    return nullptr;
}

void university::check_faculty(const std::string& name, const std::string& specialty_name) const {
    bool found = false;
    for (const faculty& value : faculties) {
        if (value.get_faculty_name() == name) {
            found = true;
            if (value.get_specialty_name() == specialty_name) {
                return;
            }
        }
    }

    if (!found) {
        throw std::out_of_range("Факультет не найден: " + name + ". Сначала создайте факультет.");
    }
    throw std::out_of_range("Специальность не найдена на факультете " + name + ": " + specialty_name + ". Сначала добавьте её.");
}

faculty& university::require_faculty(const std::string& name, const std::string& specialty_name) {
    check_faculty(name, specialty_name);
    return *find_faculty(name, specialty_name);
}

void university::check_group(const std::string& name, const std::string& specialty_name,
                             const std::string& group_name) const {
    check_faculty(name, specialty_name);
    find_faculty(name, specialty_name)->check_group(group_name);
}

bool university::add_faculty(const std::string& name, const std::string& specialty_name) {
    if (find_faculty(name, specialty_name) != nullptr) {
        throw std::logic_error("Факультет с такой специальностью уже существует.");
    }

    faculties.emplace_back(name, specialty_name);
    return true;
}

bool university::add_group(const std::string& name, const std::string& specialty_name,
                           const std::string& group_name) {
    return require_faculty(name, specialty_name).add_group(group_name);
}

bool university::remove_group(const std::string& name, const std::string& specialty_name,
                              const std::string& group_name) {
    return require_faculty(name, specialty_name).remove_group(group_name);
}

bool university::add_student(const std::string& name, const std::string& specialty_name,
                             const std::string& group_name, const student& value) {
    faculty& current_faculty = require_faculty(name, specialty_name);
    current_faculty.check_group(group_name);
    if (has_student(value.get_id())) {
        throw std::logic_error("Студент с таким UUID уже зарегистрирован (в том числе среди удалённых записей).");
    }
    return current_faculty.add_student(group_name, value);
}

bool university::remove_student(const std::string& id) {
    for (faculty& value : faculties) {
        if (value.has_student(id)) {
            return value.remove_student(id);
        }
    }

    throw std::out_of_range("Студент с указанным UUID не найден.");
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
        throw std::invalid_argument("Средний балл должен быть числом от 0 до 5.");
    }

    for (faculty& value : faculties) {
        if (value.has_student(id)) {
            return value.set_student_average_score(id, average_score);
        }
    }

    throw std::out_of_range("Студент с указанным UUID не найден.");
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
        throw std::invalid_argument("Порог успеваемости должен быть числом от 0 до 5.");
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
        throw std::invalid_argument("Порог успеваемости должен быть числом от 0 до 5.");
    }

    require_faculty(name, specialty_name).clear_students_by_score(average_threshold);
    return true;
}
