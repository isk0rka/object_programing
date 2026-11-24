#include "console.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

console::console(university& data, std::istream& input, std::ostream& output)
    : data(data), input(input), output(output)
{
}

bool console::read_line(const std::string& prompt, std::string& value) {
    output << prompt;
    return static_cast<bool>(std::getline(input, value));
}

bool console::read_score(const std::string& prompt, double& value) {
    std::string line;
    if (!read_line(prompt, line)) {
        return false;
    }

    std::istringstream stream(line);
    if (!(stream >> value) || !(stream >> std::ws).eof() ||
        !student::is_valid_average_score(value)) {
        throw std::invalid_argument("Invalid average score (0-5)");
    }

    return true;
}

void console::print_students(const std::vector<student>& students) const {
    if (students.empty()) {
        output << "No students found\n";
    }

    for (const student& value : students) {
        output << value.get_id() << " | " << value.get_full_name()
               << " | " << value.get_average_score() << '\n';
    }
}

void console::print_faculties() const {
    for (const faculty& value : data.get_faculties()) {
        output << value.get_faculty_name() << " | " << value.get_specialty_name() << '\n';
        for (const std::string& group_name : value.get_groups_list()) {
            output << "  Group: " << group_name << '\n';
            print_students(value.get_students_by_group(group_name));
        }
    }
}

bool console::execute(const std::string& command) {
    std::string faculty_name, specialty_name, group_name, id;
    double score = 0.0;
    bool success = false;

    if (command == "0") {
        return false;
    }

    if (command == "4") {
        print_faculties();
        return true;
    }

    if (command == "5") {
        if (!read_score("Average score greater than: ", score)) return false;
        print_students(data.get_students_by_score(score));
        return true;
    }

    if (command == "6" || command == "8") {
        if (!read_line("Student UUID: ", id)) return false;
        if (command == "6") {
            success = data.remove_student(id);
        } else {
            if (!read_score("New average score: ", score)) return false;
            success = data.set_student_average_score(id, score);
        }
    } else if (command == "1" || command == "2" || command == "3" ||
               command == "7" || command == "9") {
        if (!read_line("Faculty: ", faculty_name) ||
            !read_line("Specialty: ", specialty_name)) return false;

        if (command == "1") {
            success = data.add_faculty(faculty_name, specialty_name);
        } else if (command == "9") {
            if (!read_score("Delete students with score below: ", score)) return false;
            success = data.clear_students_by_score(faculty_name, specialty_name, score);
        } else {
            if (!read_line("Group: ", group_name)) return false;
            if (command == "2") {
                success = data.add_group(faculty_name, specialty_name, group_name);
            } else if (command == "7") {
                success = data.remove_group(faculty_name, specialty_name, group_name);
            } else {
                std::string surname, name, patronymic;
                if (!read_line("Surname: ", surname) || !read_line("Name: ", name) ||
                    !read_line("Patronymic: ", patronymic) ||
                    !read_score("Average score: ", score)) return false;
                success = data.add_student(faculty_name, specialty_name, group_name,
                                           student(name, surname, patronymic, score));
            }
        }
    } else if (command == "10") {
        if (!read_line("Faculty: ", faculty_name)) return false;
        print_students(data.get_students_by_faculty(faculty_name));
        return true;
    } else {
        output << "Unknown command\n";
        return true;
    }

    output << (success ? "OK\n" : "Operation rejected\n");
    return true;
}

void console::run() {
    std::string command;
    bool running = true;

    while (running) {
        output << "\n1. Add faculty\n2. Add group\n3. Add student\n4. Show university\n"
               << "5. Find students by score\n6. Delete student\n7. Delete group\n"
               << "8. Update score\n9. Clear students by score\n10. Find by faculty\n0. Exit\n";
        if (!read_line("> ", command)) {
            break;
        }

        try {
            running = execute(command);
        } catch (const std::invalid_argument& error) {
            output << error.what() << '\n';
        }
    }
}
