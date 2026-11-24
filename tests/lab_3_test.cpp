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

static student make_student(
    const std::string& group_name,
    const std::string& faculty_name,
    const std::string& specialty_name,
    double average_score
) {
    return student("Иван", "Иванов", "Иванович", group_name, faculty_name,
                   specialty_name, average_score);
}

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
    const student default_student;
    assert(default_student.get_full_name() == "Иванов Иван Иванович");
    assert(default_student.get_average_score() == 0.0);
    const student value = make_student("IT-21", "ФМФ", "Информатика", 4.75);
    assert(value.get_group_name() == "IT-21");
    assert(value.get_faculty_name() == "ФМФ");
    assert(value.get_specialty_name() == "Информатика");
    assert(value.get_average_score() == 4.75);
    assert(value.get_id().size() == 36);
    uuid_t binary_id;
    assert(uuid_parse(value.get_id().c_str(), binary_id) == 0);
    assert(default_student.get_id() != value.get_id());
    assert(student(value).get_id() == value.get_id());
    assert(make_student("IT-21", "ФМФ", "Информатика", 5).get_average_score() == 5);

    expect_exception<std::invalid_argument>([] { student("", "Иванов", "Иванович"); });
    expect_exception<std::invalid_argument>([] { student("Иван1", "Иванов", "Иванович"); });
    expect_exception<std::invalid_argument>([] { student("И ван", "Иванов", "Иванович"); });
    expect_exception<std::invalid_argument>([] { student("---", "Иванов", "Иванович"); });
    expect_exception<std::invalid_argument>([] { make_student(" ", "ФМФ", "IT", 4); });
    expect_exception<std::invalid_argument>([] { make_student("IT", "", "IT", 4); });
    expect_exception<std::invalid_argument>([] { make_student("IT", "ФМФ", "", 4); });
    for (double score : {-0.01, 5.01, std::numeric_limits<double>::infinity(),
                         std::numeric_limits<double>::quiet_NaN()}) {
        expect_exception<std::invalid_argument>([score] { make_student("IT", "ФМФ", "IT", score); });
    }

    student deleted_student = value;
    deleted_student.set_delete();
    assert(deleted_student.is_deleted());
    expect_exception<std::logic_error>([&] { deleted_student.set_delete(); });
    assert(!value.is_deleted());
}

static void test_group_and_faculty() {
    group current_group("IT-21");
    const student first = make_student("IT-21", "ФМФ", "IT", 4.5);
    const student second = make_student("IT-21", "ФМФ", "IT", 4.0);
    assert(current_group.empty());
    assert(current_group.add_student(first));
    assert(!current_group.add_student(first));
    assert(!current_group.add_student(make_student("IT-22", "ФМФ", "IT", 5)));
    assert(current_group.add_student(second));
    assert(current_group.get_student(first.get_id())->get_id() == first.get_id());
    current_group.clear_students_by_score(4.5);
    assert(current_group.get_list().size() == 1);
    assert(!current_group.get_student(second.get_id()));
    assert(current_group.has_student(second.get_id()));
    assert(!current_group.add_student(second));
    assert(current_group.remove_student(first.get_id()));
    assert(!current_group.remove_student(first.get_id()));
    assert(!current_group.empty());
    assert(current_group.get_list().empty());

    faculty current_faculty("ФМФ", "IT");
    assert(current_faculty.add_group("IT-21"));
    assert(!current_faculty.add_group("IT-21"));
    assert(current_faculty.add_group("IT-22"));
    assert(current_faculty.remove_group("IT-22"));
    assert(!current_faculty.has_group("IT-22"));
    assert(!current_faculty.remove_group("IT-22"));
    assert(current_faculty.add_student(first));
    assert(!current_faculty.add_student(make_student("IT-21", "Другой", "IT", 5)));
    assert(!current_faculty.add_student(make_student("IT-21", "ФМФ", "Математика", 5)));
    assert(!current_faculty.remove_group("IT-21"));
    assert(current_faculty.add_student(second));
    assert(current_faculty.get_groups_list().size() == 1);
    assert(current_faculty.get_students_by_score(4.0).size() == 1);
    current_faculty.clear_students_by_score(4.5);
    assert(current_faculty.get_students().size() == 1);
    assert(current_faculty.remove_student(first.get_id()));
    assert(!current_faculty.remove_group("IT-21"));
}

static void test_university() {
    university data;
    assert(data.get_students().empty());
    expect_exception<std::invalid_argument>([&] { data.get_students_by_score(-1); });
    const student first = make_student("IT-21", "ФМФ", "IT", 4.5);
    const student second = make_student("MT-21", "ФМФ", "Математика", 5);
    const student third = make_student("IT-21", "Другой", "IT", 4.0);
    const student fourth = make_student("IT-22", "ФМФ", "IT", 3.0);
    assert(data.add_student(first));
    assert(data.add_student(second));
    assert(data.add_student(third));
    assert(data.add_student(fourth));
    assert(!data.add_student(first));
    assert(data.get_students().size() == 4);
    assert(data.get_students_by_faculty("ФМФ").size() == 3);
    assert(data.get_students_by_group("IT-21").size() == 2);
    assert(data.get_students_by_specialty("IT").size() == 3);
    assert(data.get_students_by_score(4.0).size() == 2);
    assert(data.get_students_by_score(4.5).size() == 1);
    assert(data.get_students_by_score(5).empty());
    assert(data.get_students_by_faculty_and_score("ФМФ", 4.0).size() == 2);
    assert(data.get_students_by_faculty("Нет").empty());
    assert(data.get_students_by_group("Нет").empty());
    assert(data.remove_student(second.get_id()));
    assert(!data.remove_student(second.get_id()));
    assert(!data.add_student(second));
    assert(data.get_students_by_faculty("ФМФ").size() == 2);
    assert(data.get_students_by_score(4.5).empty());
    assert(data.get_students().size() == 3);
    student deleted_student;
    deleted_student.set_delete();
    assert(!data.add_student(deleted_student));
    expect_exception<std::invalid_argument>([&] { data.get_students_by_faculty_and_score("Нет", 6); });
}

static void test_console() {
    university data;
    std::istringstream input(
        "wrong\n1\nИванов\nИван\nИванович\nIT-21\nФМФ\nIT\n4.75\n"
        "2\n3\nФМФ\n4\n4.75\n5\nФМФ\n4.5\n6\nIT-21\n7\nIT\n"
        "4\n4.5abc\n4\n4,5\n4\n6\n8\nunknown\n0\n"
    );
    std::ostringstream output;
    console interface(data, input, output);
    interface.run();
    const std::string result = output.str();
    assert(result.find("Unknown command") != std::string::npos);
    assert(result.find("Student added") != std::string::npos);
    assert(result.find("Иванов Иван Иванович | IT-21 | ФМФ | IT | 4.75") != std::string::npos);
    assert(result.find("No students found") != std::string::npos);
    assert(result.find("Unexpected characters") != std::string::npos);
    assert(result.find("Average score must be a number") != std::string::npos);
    assert(result.find("Student not found") != std::string::npos);
    assert(data.get_students().size() == 1);

    std::istringstream remove_input("8\n" + data.get_students().front().get_id() + "\n0\n");
    std::ostringstream remove_output;
    console remove_interface(data, remove_input, remove_output);
    remove_interface.run();
    assert(data.get_students().empty());
    assert(remove_output.str().find("Student deleted") != std::string::npos);

    std::istringstream eof_input("1\nИванов\n");
    std::ostringstream eof_output;
    console eof_interface(data, eof_input, eof_output);
    eof_interface.run();
    assert(data.get_students().empty());
}

int main() {
    test_student();
    test_group_and_faculty();
    test_university();
    test_console();

    std::cout << "All lab_3 tests passed.\n";
    return 0;
}
