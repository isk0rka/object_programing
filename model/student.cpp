
#include "student.h"
#include "CONSTANTS.h"

#include <uuid/uuid.h>
#include <stdexcept>
#include <utility>

bool student::is_valid_fullname_part(
    const std::string& part
) {
    return !part.empty() &&
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
    std::string input_patronymic
)
    : surname(std::move(input_surname)),
      name(std::move(input_name)),
      Patronymic(std::move(input_patronymic))
{
    if (!is_valid_fullname(
        name, surname, Patronymic
    )) {
        throw std::invalid_argument(
            "Invalid full name"
        );
    }

    set_id();
}

void student::set_delete() {
    if (deleted) {
        throw std::logic_error(
            "Student already deleted"
        );
    }

    deleted = true;
}

std::string student::get_full_name() const {
    return surname + " " + name + " " + Patronymic;
}

int student::get_average_score() const {
    return average_score;
}

std::string student::get_id() const {
    return id;
}

student::~student(){

}