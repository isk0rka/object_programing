#pragma once

#include <string>

class student {
private:
    char id[37]{};

    std::string surname;
    std::string name;
    std::string Patronymic;

    double average_score = 0.0;
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
        std::string patronymic,
        double average_score = 0.0
    );

    static bool is_valid_average_score(double average_score);
    void set_average_score(double average_score);
    void set_delete();
    bool is_deleted() const;

    std::string get_full_name() const;
    double get_average_score() const;
    std::string get_id() const;
};
