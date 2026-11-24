#include "console.h"

#include <algorithm>
#include <iostream>
#include <locale>
#include <sstream>
#include <stdexcept>

static constexpr char whitespace[] = " \t\n\r\v\f";

console::console(university& data, std::istream& input, std::ostream& output)
    : data(data), input(input), output(output)
{
}

bool console::read_line(const std::string& prompt, std::string& value) {
    output << prompt << std::flush;
    if (!std::getline(input, value)) {
        if (input.bad()) {
            throw std::runtime_error("Ошибка чтения входного потока.");
        }
        return false;
    }

    const auto first = value.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        value.clear();
    } else {
        value = value.substr(first, value.find_last_not_of(whitespace) - first + 1);
    }
    return true;
}

bool console::read_required(const std::string& prompt, std::string& value) {
    if (!read_line(prompt, value)) {
        return false;
    }
    if (value.empty()) {
        throw std::invalid_argument("Поле не может быть пустым: " + prompt);
    }
    return true;
}

bool console::read_score(const std::string& prompt, double& value) {
    std::string line;
    if (!read_required(prompt, line)) {
        return false;
    }

    std::replace(line.begin(), line.end(), ',', '.');
    std::istringstream stream(line);
    stream.imbue(std::locale::classic());
    if (!(stream >> value) || !(stream >> std::ws).eof() ||
        !student::is_valid_average_score(value)) {
        throw std::invalid_argument("Введите число от 0 до 5, например 4,75 или 4.75.");
    }

    return true;
}

void console::print_students(const std::vector<student>& students) const {
    if (students.empty()) {
        output << "Студенты не найдены.\n";
        return;
    }

    output << "UUID | ФИО | Средний балл\n";
    for (const student& value : students) {
        output << value.get_id() << " | " << value.get_full_name()
               << " | " << value.get_average_score() << '\n';
    }
    output << "Всего студентов: " << students.size() << '\n';
}

void console::print_faculties() const {
    if (data.get_faculties().empty()) {
        output << "В университете пока нет факультетов. Сначала создайте факультет (пункт 1).\n";
        return;
    }

    for (const faculty& value : data.get_faculties()) {
        output << "\nФакультет: " << value.get_faculty_name()
               << " | Специальность: " << value.get_specialty_name() << '\n';
        const auto groups = value.get_groups_list();
        if (groups.empty()) {
            output << "  Нет действующих групп.\n";
        }
        for (const std::string& group_name : groups) {
            output << "  Группа: " << group_name << '\n';
            print_students(value.get_students_by_group(group_name));
        }
    }
}

bool console::execute(const std::string& command) {
    std::string faculty_name, specialty_name, group_name, id;
    double score = 0.0;

    if (command == "0") {
        output << "Работа программы завершена.\n";
        return false;
    }

    if (command == "4") {
        print_faculties();
        return true;
    }

    if (command == "5") {
        if (!read_score("Средний балл строго выше: ", score)) return false;
        print_students(data.get_students_by_score(score));
        return true;
    }

    if (command == "6" || command == "8") {
        if (!read_required("UUID студента: ", id)) return false;
        if (command == "6") {
            data.remove_student(id);
            output << "Студент удалён.\n";
        } else {
            if (!data.has_student(id)) {
                throw std::out_of_range("Студент с указанным UUID не найден.");
            }
            if (!read_score("Новый средний балл: ", score)) return false;
            data.set_student_average_score(id, score);
            output << "Средний балл обновлён.\n";
        }
    } else if (command == "1" || command == "2" || command == "3" ||
               command == "7" || command == "9") {
        if (command != "1" && data.get_faculties().empty()) {
            throw std::out_of_range("В университете нет факультетов. Сначала создайте факультет (пункт 1).");
        }
        if (!read_required("Факультет: ", faculty_name) ||
            !read_required("Специальность: ", specialty_name)) return false;

        if (command == "1") {
            data.add_faculty(faculty_name, specialty_name);
            output << "Факультет и специальность зарегистрированы.\n";
            return true;
        }

        data.check_faculty(faculty_name, specialty_name);
        if (command == "9") {
            if (!read_score("Удалить студентов с баллом строго ниже: ", score)) return false;
            std::string answer;
            if (!read_line("Подтвердите массовое удаление (да/нет): ", answer)) return false;
            if (answer != "да" && answer != "Да" && answer != "ДА") {
                output << "Удаление отменено.\n";
                return true;
            }
            data.clear_students_by_score(faculty_name, specialty_name, score);
            output << "Удаление студентов по заданному условию завершено.\n";
        } else {
            if (!read_required("Группа: ", group_name)) return false;
            if (command == "2") {
                data.add_group(faculty_name, specialty_name, group_name);
                output << "Группа создана.\n";
            } else if (command == "7") {
                data.remove_group(faculty_name, specialty_name, group_name);
                output << "Группа удалена.\n";
            } else {
                data.check_group(faculty_name, specialty_name, group_name);
                std::string surname, name, patronymic;
                if (!read_required("Фамилия: ", surname) || !read_required("Имя: ", name) ||
                    !read_required("Отчество: ", patronymic) ||
                    !read_score("Средний балл: ", score)) return false;
                student value(name, surname, patronymic, score);
                data.add_student(faculty_name, specialty_name, group_name, value);
                output << "Студент добавлен. UUID: " << value.get_id() << '\n';
            }
        }
    } else if (command == "10") {
        if (!read_required("Факультет: ", faculty_name)) return false;
        const auto& faculties = data.get_faculties();
        if (std::none_of(faculties.begin(), faculties.end(), [&](const faculty& value) {
            return value.get_faculty_name() == faculty_name;
        })) {
            throw std::out_of_range("Факультет не найден: " + faculty_name);
        }
        print_students(data.get_students_by_faculty(faculty_name));
    } else {
        throw std::invalid_argument("Неизвестная команда. Выберите пункт меню от 0 до 10.");
    }

    return true;
}

void console::run() {
    std::string command;
    bool running = true;

    while (running) {
        output << "\n1. Добавить факультет и специальность\n2. Добавить группу\n"
               << "3. Добавить студента\n4. Показать университет\n"
               << "5. Найти студентов по среднему баллу\n6. Удалить студента\n"
               << "7. Удалить группу\n8. Изменить средний балл\n"
               << "9. Удалить студентов по среднему баллу\n10. Найти студентов по факультету\n0. Выход\n";
        if (!read_line("> ", command)) {
            output << "Ввод завершён.\n";
            break;
        }

        try {
            running = execute(command);
            if (!running && command != "0") {
                output << "Ввод завершён. Незаконченная операция отменена.\n";
            }
        } catch (const std::invalid_argument& error) {
            output << "Ошибка ввода: " << error.what() << '\n';
        } catch (const std::out_of_range& error) {
            output << "Объект не найден: " << error.what() << '\n';
        } catch (const std::logic_error& error) {
            output << "Операция запрещена: " << error.what() << '\n';
        }
    }
}
