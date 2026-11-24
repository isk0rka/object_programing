
#include "student.h"
#include "CONSTANTS.h"

#include <uuid/uuid.h>
#include <cmath>
#include <stdexcept>
#include <utility>

bool student::is_valid_fullname_part(
    const std::string& part
) {
    return !part.empty() &&
           part.find('\0') == std::string::npos &&
           part.find_first_of(FORBIDDEN_FULLNAME_CHARS)
               == std::string::npos;
}

bool student::is_valid_fullname(
    const std::string& name,
    const std::string& surname,
    const std::string& patronymic
) {
    return is_valid_fullname_part(name) &&
           is_valid_fullname_part(surname) &&
           is_valid_fullname_part(patronymic);
}

void student::set_id() {
    uuid_t binary_id;

    uuid_generate_random(binary_id);
    uuid_unparse_lower(binary_id, id);
}

student::student()
    : student("Иван", "Иванов", "Иванович")
{
}

student::student(
    std::string input_name,
    std::string input_surname,
    std::string input_patronymic,
    double input_average_score
)
    : surname(std::move(input_surname)),
      name(std::move(input_name)),
      Patronymic(std::move(input_patronymic))
{
    if (!is_valid_fullname(
        name, surname, Patronymic
    )) {
        throw std::invalid_argument(
            "Некорректное ФИО: каждая часть должна быть непустой и не содержать запрещённых символов."
        );
    }

    set_average_score(input_average_score);
    set_id();
}

bool student::is_valid_average_score(double average_score) {
    return std::isfinite(average_score) &&
           average_score >= 0.0 && average_score <= 5.0;
}

void student::set_average_score(double average_score) {
    if (deleted) {
        throw std::logic_error("Студент уже удалён.");
    }

    if (!is_valid_average_score(average_score)) {
        throw std::invalid_argument("Средний балл должен быть числом от 0 до 5.");
    }

    this->average_score = average_score;
}

void student::set_delete() {
    if (deleted) {
        throw std::logic_error(
            "Студент уже удалён."
        );
    }

    deleted = true;
}

bool student::is_deleted() const {
    return deleted;
}

std::string student::get_full_name() const {
    return surname + " " + name + " " + Patronymic;
}

double student::get_average_score() const {
    return average_score;
}

std::string student::get_id() const {
    return id;
}
