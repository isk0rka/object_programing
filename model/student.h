/* «Студент», который определяется полями ФИО, номер группы, название факультета, 
название специальности, средний балл успеваемости. */
#pragma once

#include <string>

class student {
private:
    char id[37]{};

    std::string surname;
    std::string name;
    std::string Patronymic;

    int average_score = 0;
    bool deleted = false;

    static bool is_valid_fullname_part(
        const std::string& name_part
    );

    static bool is_valid_fullname(
        const std::string& name,
        const std::string& surname,
        const std::string& patronymic
    );

    void set_id();

public:
    student();

    student(
        std::string name,
        std::string surname,
        std::string patronymic
    );

    void set_delete();

    std::string get_full_name() const;
    int get_average_score() const;
    std::string get_id() const;
};