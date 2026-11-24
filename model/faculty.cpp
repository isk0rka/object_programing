#include "faculty.h"

#include <stdexcept>

faculty::faculty()
    : faculty("Не указан", "Не указана")
{
}

faculty::faculty(const std::string& name, const std::string& specialty_name)
    : name(name), specialty_name(specialty_name)
{
    if (name.find_first_not_of(" \t\n\r\v\f") == std::string::npos ||
        specialty_name.find_first_not_of(" \t\n\r\v\f") == std::string::npos) {
        throw std::invalid_argument("Названия факультета и специальности не могут быть пустыми.");
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
            if (value.is_deleted()) {
                throw std::logic_error("Группа с таким названием была удалена. Повторное создание запрещено: " + group_name);
            }
            throw std::logic_error("Группа уже существует: " + group_name);
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

    throw std::out_of_range("Группа не найдена: " + group_name);
}

bool faculty::has_group(const std::string& group_name) const {
    for (const group& value : groups) {
        if (!value.is_deleted() && value.get_group_name() == group_name) {
            return true;
        }
    }

    return false;
}

void faculty::check_group(const std::string& group_name) const {
    for (const group& value : groups) {
        if (value.get_group_name() == group_name) {
            if (value.is_deleted()) {
                throw std::logic_error("Группа удалена: " + group_name);
            }
            return;
        }
    }

    throw std::out_of_range("Группа не найдена: " + group_name + ". Сначала создайте группу.");
}

bool faculty::add_student(const std::string& group_name, const student& value) {
    check_group(group_name);
    if (has_student(value.get_id())) {
        throw std::logic_error("Студент с таким UUID уже зарегистрирован (в том числе среди удалённых записей).");
    }

    for (group& current_group : groups) {
        if (current_group.get_group_name() == group_name) {
            return current_group.add_student(value);
        }
    }

    throw std::out_of_range("Группа не найдена: " + group_name);
}

bool faculty::remove_student(const std::string& id) {
    for (group& value : groups) {
        if (value.has_student(id)) {
            return value.remove_student(id);
        }
    }

    throw std::out_of_range("Студент с указанным UUID не найден.");
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
        throw std::invalid_argument("Средний балл должен быть числом от 0 до 5.");
    }

    for (group& value : groups) {
        if (value.has_student(id)) {
            return value.set_student_average_score(id, average_score);
        }
    }

    throw std::out_of_range("Студент с указанным UUID не найден.");
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
        throw std::invalid_argument("Порог успеваемости должен быть числом от 0 до 5.");
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
        throw std::invalid_argument("Порог успеваемости должен быть числом от 0 до 5.");
    }

    for (group& value : groups) {
        value.clear_students_by_score(average_threshold);
    }
}
