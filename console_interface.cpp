#include "console_interface.h"

#include <iostream>
#include <sstream>
#include <string>
#include <limits>

using namespace std;

bool console_interface::try_input_positive_double(double& value) {
    string input;
    getline(cin, input);

    stringstream stream(input);

    if (!(stream >> value)) {
        return false;
    }

    if (value <= 0) {
        return false;
    }

    stream >> ws;

    if (!stream.eof()) {
        return false;
    }

    return true;
}

int console_interface::input_positive_int() {
    string input;
    int value = 0;
    bool correct_input = false;

    while (!correct_input) {
        getline(cin, input);

        stringstream stream(input);

        if (stream >> value) {
            stream >> ws;

            if (stream.eof() && value > 0) {
                correct_input = true;
            }
        }

        if (!correct_input) {
            cout << "Ошибка. Введите положительное целое число: ";
        }
    }

    return value;
}

void console_interface::input_pyramids(pyramids_manager& manager) {
    cout << "Введите количество пирамид: ";
    int count = input_positive_int();

    int i = 0;

    while (i < count) {
        double side;
        double height;

        cout << "\nПирамида #" << i + 1 << '\n';

        cout << "Введите сторону основания: ";

        if (!try_input_positive_double(side)) {
            cout << "Ошибка ввода. Повторите ввод пирамиды.\n";
            continue;
        }

        cout << "Введите высоту: ";

        if (!try_input_positive_double(height)) {
            cout << "Ошибка ввода. Повторите ввод пирамиды.\n";
            continue;
        }

        manager.add_pyramid(side, height);

        i++;
    }
}

void console_interface::print_pyramid(const pyramid& object) {
    cout << "Сторона основания: " << object.get_side() << '\n';

    cout << "Высота: " << object.get_height() << '\n';

    cout << "Площадь основания: " << object.get_base_area() << '\n';

    cout << "Объем: " << object.get_volume() << '\n';
}

void console_interface::run() {
    pyramids_manager manager;

    input_pyramids(manager);

    cout << "\nПирамида с наибольшей площадью основания:\n";
    print_pyramid(manager.get_max_base_area());

    cout << "\nПирамида с наибольшим объемом:\n";
    print_pyramid(manager.get_max_volume());
}