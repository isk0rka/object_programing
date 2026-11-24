#pragma once

#include <string>
#include <vector>
#include "student.h"
#include "group.h"

class faculty {
private:
    std::string name;
    std::string specialty_name;
    std::vector<group> groups;

public:
    faculty();
    faculty(const std::string& name, const std::string& specialty_name);

    std::string get_faculty_name() const;
    std::string get_specialty_name() const;

    bool add_group(const std::string& group);
    bool remove_group(const std::string& group);
    bool has_group(const std::string& group) const;
    void check_group(const std::string& group_name) const;

    bool add_student(const std::string& group_name, const student& value);
    bool remove_student(const std::string& id);
    bool has_student(const std::string& id) const;
    bool set_student_average_score(const std::string& id, double average_score);

    std::vector<std::string> get_groups_list() const;
    std::vector<student> get_students_by_group(const std::string& group_name) const;
    std::vector<student> get_students() const;
    std::vector<student> get_students_by_score(const double average_threshold) const;

    void clear_students_by_score(const double average_threshold);
};
