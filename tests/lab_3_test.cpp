#include "../app/university.h"
#include "../infrastructure/console.h"
#include "../model/group.h"
#include "../model/student.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <uuid/uuid.h>

template <typename exception, typename function>
static void expect_exception(function action) {
    bool thrown = false;
    try {
        action();
    } catch (const exception&) {
        thrown = true;
    }
    assert(thrown);
}

static void test_student() {
    student first;
    assert(first.get_full_name() == "Иванов Иван Иванович");
    assert(first.get_average_score() == 0.0);
    const student second("Пётр", "Петров", "Петрович", 4.75);
    assert(second.get_full_name() == "Петров Пётр Петрович");
    assert(second.get_average_score() == 4.75);
    assert(first.get_id() != second.get_id());
    uuid_t binary_id;
    assert(uuid_parse(second.get_id().c_str(), binary_id) == 0);
    assert(student(second).get_id() == second.get_id());
    first.set_average_score(5);
    assert(first.get_average_score() == 5);
    for (double score : {-0.01, 5.01, std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::quiet_NaN()}) {
        expect_exception<std::invalid_argument>([&] { first.set_average_score(score); });
        expect_exception<std::invalid_argument>([&] { student("Иван", "Иванов", "Иванович", score); });
        assert(first.get_average_score() == 5);
    }
    expect_exception<std::invalid_argument>([] { student("", "Иванов", "Иванович"); });
    expect_exception<std::invalid_argument>([] { student("Иван1", "Иванов", "Иванович"); });
    first.set_delete();
    assert(first.is_deleted());
    expect_exception<std::logic_error>([&] { first.set_delete(); });
    expect_exception<std::logic_error>([&] { first.set_average_score(4); });
}

static void test_group() {
    group data("ИТ-21");
    const student first("Иван", "Иванов", "Иванович", 4.5);
    const student second("Пётр", "Петров", "Петрович", 4);
    student deleted;
    deleted.set_delete();
    assert(data.empty());
    assert(!data.is_deleted());
    assert(data.add_student(first));
    expect_exception<std::logic_error>([&] { data.add_student(first); });
    expect_exception<std::logic_error>([&] { data.add_student(deleted); });
    assert(data.add_student(second));
    expect_exception<std::logic_error>([&] { data.set_delete(); });
    assert(data.get_list().size() == 2);
    assert(data.get_student(first.get_id())->get_id() == first.get_id());
    assert(!data.get_student("missing"));
    assert(data.get_students_by_score(4).size() == 1);
    assert(data.get_students_by_score(4.5).empty());
    assert(data.set_student_average_score(second.get_id(), 4.25));
    assert(data.get_students_by_score(4).size() == 2);
    data.clear_students_by_score(4.5);
    assert(data.get_list().size() == 1);
    assert(data.has_student(second.get_id()));
    assert(!data.get_student(second.get_id()));
    expect_exception<std::logic_error>([&] { data.add_student(second); });
    expect_exception<std::logic_error>([&] { data.set_student_average_score(second.get_id(), 5); });
    assert(data.remove_student(first.get_id()));
    expect_exception<std::logic_error>([&] { data.remove_student(first.get_id()); });
    assert(data.empty());
    assert(data.set_delete());
    assert(data.is_deleted());
    expect_exception<std::logic_error>([&] { data.set_delete(); });
    expect_exception<std::logic_error>([&] { data.add_student(student()); });
    assert(data.has_student(first.get_id()));
    assert(data.get_list().empty());
    expect_exception<std::invalid_argument>([&] { data.clear_students_by_score(-1); });
    expect_exception<std::invalid_argument>([] { group(" "); });
}

static void test_faculty() {
    faculty data("ФМФ", "Информатика");
    assert(data.get_faculty_name() == "ФМФ");
    assert(data.get_specialty_name() == "Информатика");
    assert(data.add_group("ИТ-21"));
    assert(data.add_group("ИТ-22"));
    expect_exception<std::logic_error>([&] { data.add_group("ИТ-21"); });
    const student first("Иван", "Иванов", "Иванович", 4.5);
    const student second("Пётр", "Петров", "Петрович", 4);
    expect_exception<std::out_of_range>([&] { data.add_student("unknown", first); });
    assert(data.add_student("ИТ-21", first));
    expect_exception<std::logic_error>([&] { data.add_student("ИТ-22", first); });
    assert(data.add_student("ИТ-22", second));
    expect_exception<std::logic_error>([&] { data.remove_group("ИТ-21"); });
    assert(data.get_students().size() == 2);
    assert(data.get_students_by_score(4).size() == 1);
    assert(data.get_students_by_group("ИТ-22").size() == 1);
    assert(data.get_students_by_group("unknown").empty());
    data.clear_students_by_score(4.5);
    assert(data.get_students().size() == 1);
    assert(data.remove_group("ИТ-22"));
    assert(!data.has_group("ИТ-22"));
    expect_exception<std::logic_error>([&] { data.add_group("ИТ-22"); });
    assert(data.has_student(second.get_id()));
    expect_exception<std::logic_error>([&] { data.add_student("ИТ-21", second); });
    assert(data.get_groups_list().size() == 1);
    assert(data.set_student_average_score(first.get_id(), 5));
    assert(data.get_students_by_score(4.5).size() == 1);
    expect_exception<std::invalid_argument>([] { faculty("ФМФ", ""); });
    const faculty empty;
    expect_exception<std::invalid_argument>([&] { empty.get_students_by_score(6); });
}

static void test_university() {
    university data;
    const student first("Иван", "Иванов", "Иванович", 4.5);
    const student second("Пётр", "Петров", "Петрович", 5);
    expect_exception<std::out_of_range>([&] { data.add_group("ФМФ", "IT", "ИТ-21"); });
    expect_exception<std::out_of_range>([&] { data.add_student("ФМФ", "IT", "ИТ-21", first); });
    assert(data.add_faculty("ФМФ", "IT"));
    expect_exception<std::logic_error>([&] { data.add_faculty("ФМФ", "IT"); });
    assert(data.add_faculty("ФМФ", "Math"));
    expect_exception<std::out_of_range>([&] { data.add_student("ФМФ", "IT", "ИТ-21", first); });
    assert(data.add_group("ФМФ", "IT", "ИТ-21"));
    assert(data.add_group("ФМФ", "Math", "МТ-21"));
    assert(data.add_student("ФМФ", "IT", "ИТ-21", first));
    expect_exception<std::logic_error>([&] { data.add_student("ФМФ", "Math", "МТ-21", first); });
    assert(data.add_student("ФМФ", "Math", "МТ-21", second));
    assert(data.get_faculties().size() == 2);
    assert(data.get_students_by_faculty("ФМФ").size() == 2);
    assert(data.get_students_by_score(4.5).size() == 1);
    assert(data.set_student_average_score(first.get_id(), 4.75));
    assert(data.get_students_by_score(4.5).size() == 2);
    assert(data.clear_students_by_score("ФМФ", "IT", 4.8));
    assert(data.get_students_by_faculty("ФМФ").size() == 1);
    assert(data.remove_group("ФМФ", "IT", "ИТ-21"));
    assert(data.has_student(first.get_id()));
    expect_exception<std::logic_error>([&] { data.add_student("ФМФ", "Math", "МТ-21", first); });
    expect_exception<std::logic_error>([&] { data.remove_student(first.get_id()); });
    assert(data.remove_student(second.get_id()));
    assert(data.get_students_by_faculty("ФМФ").empty());
    expect_exception<std::invalid_argument>([&] { data.get_students_by_score(-1); });
    expect_exception<std::invalid_argument>([&] { data.set_student_average_score("missing", 6); });
}

static void test_diagnostics() {
    university data;
    const student value("Иван", "Иванов", "Иванович", 4.5);
    const auto expect_message = [](auto action, const std::string& message) {
        bool thrown = false;
        try {
            action();
        } catch (const std::exception& error) {
            thrown = true;
            assert(std::string(error.what()).find(message) != std::string::npos);
        }
        assert(thrown);
    };
    expect_message([&] { data.add_student("ФМФ", "ИТ", "Г1", value); }, "Факультет не найден");
    assert(data.get_faculties().empty());
    data.add_faculty("ФМФ", "ИТ");
    expect_message([&] { data.add_student("ФМФ", "Математика", "Г1", value); }, "Специальность не найдена");
    expect_message([&] { data.add_student("ФМФ", "ИТ", "Г1", value); }, "Группа не найдена");
    assert(data.get_faculties().front().get_groups_list().empty());
    data.add_group("ФМФ", "ИТ", "Г1");
    data.add_group("ФМФ", "ИТ", "Г2");
    data.add_student("ФМФ", "ИТ", "Г1", value);
    expect_message([&] { data.remove_group("ФМФ", "ИТ", "Г1"); }, "действующими студентами");
    expect_message([&] { data.add_student("ФМФ", "ИТ", "Г2", value); }, "уже зарегистрирован");
    assert(data.get_students_by_score(0).size() == 1);
    data.remove_group("ФМФ", "ИТ", "Г2");
    expect_message([&] { data.add_student("ФМФ", "ИТ", "Г2", student()); }, "Группа удалена");
    expect_message([&] { data.remove_group("ФМФ", "ИТ", "Г2"); }, "Группа уже удалена");
    expect_message([&] { data.add_group("ФМФ", "ИТ", "Г2"); }, "Повторное создание запрещено");
    expect_message([&] { data.remove_student("missing"); }, "не найден");
    data.remove_student(value.get_id());
    expect_message([&] { data.remove_student(value.get_id()); }, "Студент уже удалён");
    expect_message([&] { data.set_student_average_score(value.get_id(), 5); }, "Студент уже удалён");
    assert(data.get_students_by_score(0).empty());
    assert(data.has_student(value.get_id()));
}

static std::string run_console(university& data, const std::string& commands) {
    std::istringstream input(commands);
    std::ostringstream output;
    console interface(data, input, output);
    interface.run();
    return output.str();
}

static void test_console() {
    university data;
    const auto empty = run_console(data, "3\n4\n5\n1\n0\n");
    assert(empty.find("В университете нет факультетов") != std::string::npos);
    assert(empty.find("В университете пока нет факультетов") != std::string::npos);
    assert(empty.find("Фамилия:") == std::string::npos);
    assert(empty.find("Факультет: ") == std::string::npos);
    assert(empty.find("Студенты не найдены") != std::string::npos);
    assert(data.get_faculties().empty());

    const auto result = run_console(data,
        "wrong\n1\n  ФМФ  \nИТ\n4\n"
        "1\nФМФ\nИТ\n"
        "3\nНет\nИТ\n3\nФМФ\nНет\n3\nФМФ\nИТ\nНет\n"
        "2\nФМФ\nИТ\nГ1\n2\nФМФ\nИТ\nГ1\n"
        "3\nФМФ\nИТ\nГ1\nИванов\nИван1\nИванович\n4\n"
        "3\nФМФ\nИТ\nГ1\nИванов\nИван\nИванович\n4,75\n"
        "4\n5\n4.75\n5\n4.5abc\n5\n4,5.1\n5\n6\n5\nnan\n5\ninf\n"
        "7\nФМФ\nИТ\nГ1\n10\nНет\n10\nФМФ\n"
        "9\nФМФ\nИТ\n5\nнет\n6\nunknown\n8\nunknown\n0\n");
    assert(data.get_students_by_faculty("ФМФ").size() == 1);
    for (const std::string message : {"Неизвестная команда", "Нет действующих групп",
         "уже существует", "Факультет не найден", "Специальность не найдена", "Группа не найдена",
         "Некорректное ФИО", "Студент добавлен", "Иванов Иван Иванович | 4.75",
         "Студенты не найдены", "Введите число от 0 до 5", "действующими студентами",
         "Удаление отменено", "Студент с указанным UUID не найден", "Работа программы завершена"}) {
        assert(result.find(message) != std::string::npos);
    }
    const auto id = data.get_students_by_faculty("ФМФ").front().get_id();
    const auto updated = run_console(data, "8\n" + id + "\n3,5\n0\n");
    assert(updated.find("Средний балл обновлён") != std::string::npos);
    assert(data.get_students_by_faculty("ФМФ").front().get_average_score() == 3.5);
    const auto removed = run_console(data, "9\nФМФ\nИТ\n4\nда\n6\n" + id + "\n7\nФМФ\nИТ\nГ1\n0\n");
    assert(removed.find("Студент уже удалён") != std::string::npos);
    assert(removed.find("Группа удалена") != std::string::npos);
    assert(data.get_students_by_faculty("ФМФ").empty());
    assert(data.get_faculties().front().get_groups_list().empty());
    data.add_group("ФМФ", "ИТ", "Г2");
    const auto eof = run_console(data, "3\nФМФ\nИТ\nГ2\nИванов\n");
    assert(eof.find("Незаконченная операция отменена") != std::string::npos);
    assert(data.get_students_by_faculty("ФМФ").empty());
}

int main() {
    test_student();
    test_group();
    test_faculty();
    test_university();
    test_diagnostics();
    test_console();
    std::cout << "Все тесты лабораторной работы №3 пройдены.\n";
    return 0;
}
