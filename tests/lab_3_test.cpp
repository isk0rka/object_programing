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
    assert(!data.add_student(first));
    assert(!data.add_student(deleted));
    assert(data.add_student(second));
    assert(!data.set_delete());
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
    assert(!data.add_student(second));
    assert(!data.set_student_average_score(second.get_id(), 5));
    assert(data.remove_student(first.get_id()));
    assert(!data.remove_student(first.get_id()));
    assert(data.empty());
    assert(data.set_delete());
    assert(data.is_deleted());
    assert(!data.set_delete());
    assert(!data.add_student(student()));
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
    assert(!data.add_group("ИТ-21"));
    const student first("Иван", "Иванов", "Иванович", 4.5);
    const student second("Пётр", "Петров", "Петрович", 4);
    assert(!data.add_student("unknown", first));
    assert(data.add_student("ИТ-21", first));
    assert(!data.add_student("ИТ-22", first));
    assert(data.add_student("ИТ-22", second));
    assert(!data.remove_group("ИТ-21"));
    assert(data.get_students().size() == 2);
    assert(data.get_students_by_score(4).size() == 1);
    assert(data.get_students_by_group("ИТ-22").size() == 1);
    assert(data.get_students_by_group("unknown").empty());
    data.clear_students_by_score(4.5);
    assert(data.get_students().size() == 1);
    assert(data.remove_group("ИТ-22"));
    assert(!data.has_group("ИТ-22"));
    assert(!data.add_group("ИТ-22"));
    assert(data.has_student(second.get_id()));
    assert(!data.add_student("ИТ-21", second));
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
    assert(!data.add_group("ФМФ", "IT", "ИТ-21"));
    assert(!data.add_student("ФМФ", "IT", "ИТ-21", first));
    assert(data.add_faculty("ФМФ", "IT"));
    assert(!data.add_faculty("ФМФ", "IT"));
    assert(data.add_faculty("ФМФ", "Math"));
    assert(!data.add_student("ФМФ", "IT", "ИТ-21", first));
    assert(data.add_group("ФМФ", "IT", "ИТ-21"));
    assert(data.add_group("ФМФ", "Math", "МТ-21"));
    assert(data.add_student("ФМФ", "IT", "ИТ-21", first));
    assert(!data.add_student("ФМФ", "Math", "МТ-21", first));
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
    assert(!data.add_student("ФМФ", "Math", "МТ-21", first));
    assert(!data.remove_student(first.get_id()));
    assert(data.remove_student(second.get_id()));
    assert(data.get_students_by_faculty("ФМФ").empty());
    expect_exception<std::invalid_argument>([&] { data.get_students_by_score(-1); });
    expect_exception<std::invalid_argument>([&] { data.set_student_average_score("missing", 6); });
}

static void test_console() {
    university data;
    std::istringstream input(
        "unknown\n2\nФМФ\nIT\nИТ-21\n1\nФМФ\nIT\n2\nФМФ\nIT\nИТ-21\n"
        "3\nФМФ\nIT\nИТ-21\nИванов\nИван\nИванович\n4.75\n4\n5\n4.75\n"
        "5\n4.5abc\n5\n4,5\n5\n6\n10\nФМФ\n0\n"
    );
    std::ostringstream output;
    console interface(data, input, output);
    interface.run();
    assert(data.get_students_by_faculty("ФМФ").size() == 1);
    const std::string result = output.str();
    assert(result.find("Unknown command") != std::string::npos);
    assert(result.find("Operation rejected") != std::string::npos);
    assert(result.find("Иванов Иван Иванович | 4.75") != std::string::npos);
    assert(result.find("No students found") != std::string::npos);
    assert(result.find("Invalid average score") != std::string::npos);
    std::istringstream remove_input("6\n" + data.get_students_by_faculty("ФМФ").front().get_id() + "\n0\n");
    std::ostringstream remove_output;
    console remove_interface(data, remove_input, remove_output);
    remove_interface.run();
    assert(data.get_students_by_faculty("ФМФ").empty());
    std::istringstream eof_input("3\nФМФ\n");
    std::ostringstream eof_output;
    console eof_interface(data, eof_input, eof_output);
    eof_interface.run();
    assert(data.get_students_by_faculty("ФМФ").empty());
}

int main() {
    test_student();
    test_group();
    test_faculty();
    test_university();
    test_console();
    std::cout << "All lab_3 tests passed.\n";
    return 0;
}
