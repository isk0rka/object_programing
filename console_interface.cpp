#include "console_interface.h"

#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <string>

using namespace std;

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

bool console_interface::valid_input_pyramids(double side,double height) {
    try
        {
            manager.add_pyramid(side, height);
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            return false;
        }
}

void console_interface::input_pyramids() {
    double side = -1.0;
    double height = -1.0;

    cout << "Введите количество пирамид: ";

    int count = input_positive_int();

    for (int i = 0; i < count; ++i) {
        do {
            cout << "Введите сторону основания: ";
            cin >> side;
            cout << "Введите высоту: ";
            cin >> height;
        } while (!valid_input_pyramids(side, height));
    }
}


void console_interface::print_pyramid(const pyramid& object) {
    cout << "Сторона основания: " << object.get_side() << '\n';

    cout << "Высота: " << object.get_height() << '\n';

    cout << "Площадь основания: " << object.get_base_area() << '\n';

    cout << "Объем: " << object.get_volume() << '\n';
}


void console_interface::print_all_pyramids() {
    cout << "\nВсе пирамиды:\n\n";

    cout << fixed << setprecision(2);

    cout << left
         << setw(8)  << "#"
         << setw(12) << "Side"
         << setw(12) << "Height"
         << setw(12) << "Area"
         << setw(12) << "Volume"
         << '\n';

    cout << string(56, '-') << '\n';

    for (std::size_t i = 0; i < manager.get_count(); ++i) {
        const pyramid* object = manager.get_pyramid(i);

        if (object != nullptr) {
            cout << left
                 << setw(8)  << i + 1
                 << setw(12) << object->get_side()
                 << setw(12) << object->get_height()
                 << setw(12) << object->get_base_area()
                 << setw(12) << object->get_volume()
                 << '\n';
        }
    }
}


void console_interface::run() {
    input_pyramids();

    print_all_pyramids();

    const pyramid* max_base_area =
        manager.get_max_base_area();

    cout << "\nПирамида с наибольшей площадью основания:\n";

    if (max_base_area != nullptr) {
        print_pyramid(*max_base_area);
    }

    const pyramid* max_volume =
        manager.get_max_volume();

    cout << "\nПирамида с наибольшим объемом:\n";

    if (max_volume != nullptr) {
        print_pyramid(*max_volume);
    }
}