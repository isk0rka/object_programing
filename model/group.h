#pragma once

#include <optional>
#include <string>
#include <vector>

#include "student.h"

class group {
private:
    std::string name;
    std::vector<student> list;

    bool deleted = false;

public:
    group();
    explicit group(const std::string& name);

    std::string get_group_name() const;
    bool is_deleted() const;
    bool empty() const;
    bool set_delete();

    bool add_student(const student& value);
    bool remove_student(const std::string& id);
    bool has_student(const std::string& id) const;
    bool set_student_average_score(const std::string& id, double average_score);

    std::vector<student> get_list() const;
    std::optional<student> get_student(const std::string& id) const;
    std::vector<student> get_students_by_score(double average_threshold) const;

    void clear_students_by_score(double average_threshold);
};
