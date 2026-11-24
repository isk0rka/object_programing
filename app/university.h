#pragma once

#include <string>
#include <vector>

#include "../model/faculty.h"

class university {
private:
    std::vector<faculty> faculties;

    faculty* find_faculty(const std::string& name, const std::string& specialty_name);

public:
    bool add_faculty(const std::string& name, const std::string& specialty_name);
    bool add_group(const std::string& name, const std::string& specialty_name,
                   const std::string& group_name);
    bool remove_group(const std::string& name, const std::string& specialty_name,
                      const std::string& group_name);

    bool add_student(const std::string& name, const std::string& specialty_name,
                     const std::string& group_name, const student& value);
    bool remove_student(const std::string& id);
    bool has_student(const std::string& id) const;
    bool set_student_average_score(const std::string& id, double average_score);

    const std::vector<faculty>& get_faculties() const;
    std::vector<student> get_students_by_faculty(const std::string& name) const;
    std::vector<student> get_students_by_score(double average_threshold) const;
    bool clear_students_by_score(const std::string& name, const std::string& specialty_name,
                                 double average_threshold);
};
