#include "Car.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

bool read_positive_int(const std::string& text, int& value) {
    std::string line;

    while (std::cout << text << std::flush &&
           std::getline(std::cin, line)) {

        std::istringstream input(line);
        std::string extra;
        int number;

        if ((input >> number) &&
            number > 0 &&
            !(input >> extra)) {

            value = number;
            return true;
        }

        std::cout
            << "Ошибка: введите положительное целое число.\n";
    }

    return false;
}

void move_car(Car& car, int target) {
    car.set_speed(target);

    std::cout << "Целевая скорость: "
              << car.get_target_speed()
              << " км/ч\n";

    char previous_gear = car.get_gear();

    std::cout << "Передачи: "
              << previous_gear
              << std::flush;

    while (car.update()) {
        const char current_gear = car.get_gear();

        if (current_gear != previous_gear) {
            std::cout << " -> "
                      << current_gear
                      << std::flush;

            previous_gear = current_gear;
        }

        if (car.get_speed() != car.get_target_speed()) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(30)
            );
        }
    }

    std::cout << "\nСкорость достигнута: "
              << car.get_speed()
              << " км/ч\n";
}

void show_status(const Car& car) {
    std::cout << "Двигатель: "
              << (car.is_engine_working()
                  ? "включён"
                  : "выключен")
              << "\nСкорость: "
              << car.get_speed()
              << " км/ч"
              << "\nЦелевая скорость: "
              << car.get_target_speed()
              << " км/ч"
              << "\nПередача: "
              << car.get_gear()
              << '\n';
}

int main() {
    int max_forward;
    int max_back;

    if (!read_positive_int(
            "Максимальная скорость вперёд (км/ч): ",
            max_forward
        ) ||
        !read_positive_int(
            "Максимальная скорость назад (км/ч): ",
            max_back
        )) {

        return 0;
    }

    try {
        Car car(max_forward, max_back);

        std::cout
            << "\nАвтомобиль создан.\n"
            << "Команды: on_eng, off_eng, "
            << "speed <км/ч>, status, exit\n";

        std::string line;

        while (std::cout << "\n> " << std::flush &&
               std::getline(std::cin, line)) {

            std::istringstream input(line);
            std::string command;
            std::string extra;

            if (!(input >> command)) {
                continue;
            }

            try {
                if (command == "speed") {
                    int target;

                    if (!(input >> target) ||
                        (input >> extra)) {

                        throw std::invalid_argument(
                            "Укажите одну целую скорость."
                        );
                    }

                    move_car(car, target);
                    continue;
                }

                if (input >> extra) {
                    throw std::invalid_argument(
                        "Лишние аргументы команды."
                    );
                }

                if (command == "on_eng") {
                    car.set_engine_work(true);

                    std::cout
                        << "Двигатель включён."
                        << std::endl;

                } else if (command == "off_eng") {
                    car.set_engine_work(false);

                    std::cout
                        << "Двигатель выключен."
                        << std::endl;

                } else if (command == "status") {
                    show_status(car);

                } else if (command == "exit") {
                    std::cout
                        << "Программа завершена.\n";

                    break;

                } else {
                    throw std::invalid_argument(
                        "Неизвестная команда."
                    );
                }

            } catch (const std::exception& error) {
                std::cout << "Ошибка: "
                          << error.what()
                          << '\n';
            }
        }

    } catch (const std::exception& error) {
        std::cerr << "Ошибка: "
                  << error.what()
                  << '\n';

        return 1;
    }

    return 0;
}