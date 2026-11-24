#pragma once

#include <iosfwd>
#include <string>
#include <vector>

#include "../app/university.h"

class console {
private:
    university& data;
    std::istream& input;
    std::ostream& output;

    bool read_line(const std::string& prompt, std::string& value);
    bool read_score(const std::string& prompt, double& value);
    void print_students(const std::vector<student>& students) const;
    void print_faculties() const;
    bool execute(const std::string& command);

public:
    console(university& data, std::istream& input, std::ostream& output);
    void run();
};
