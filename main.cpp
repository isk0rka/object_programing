#include "Car.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

void move_car(Car& car, int target) {
    car.set_speed(target);

    char previous_gear = car.get_gear();

    std::cout << "Передачи: " << previous_gear;

    while (car.update()) {
        const char current_gear = car.get_gear();

        if (current_gear != previous_gear) {
            std::cout << " -> "
                      << current_gear
                      << std::flush;

            previous_gear = current_gear;
        }

        std::this_thread::sleep_for(
            std::chrono::milliseconds(30)
        );
    }

    std::cout << "\nСкорость: "
              << car.get_speed()
              << " км/ч\n";
}

int main() {
    int max_forward;
    int max_back;

    std::cout << "Максимальные скорости вперёд и назад: ";

    if (!(std::cin >> max_forward >> max_back)) {
        std::cerr << "Некорректный ввод.\n";
        return 1;
    }

    try {
        Car car(max_forward, max_back);

        std::string line;
        std::getline(std::cin, line);

        std::cout
            << "Команды: on, off, speed <км/ч>, status, exit\n";

        while (std::cout << "> " &&
               std::getline(std::cin, line)) {

            std::istringstream input(line);
            std::string command;

            if (!(input >> command))
                continue;

            try {
                if (command == "speed") {
                    int target;
                    std::string extra;

                    if (!(input >> target) || (input >> extra)) {
                        throw std::invalid_argument(
                            "Укажите одну целую скорость."
                        );
                    }

                    move_car(car, target);
                } else {
                    std::string extra;

                    if (input >> extra) {
                        throw std::invalid_argument(
                            "Лишние аргументы команды."
                        );
                    }

                    if (command == "on") {
                        car.set_engine_work(true);
                    } else if (command == "off") {
                        car.set_engine_work(false);
                    } else if (command == "status") {
                        std::cout
                            << "Двигатель: "
                            << (car.is_engine_working()
                                ? "включён"
                                : "выключен")
                            << ", скорость: "
                            << car.get_speed()
                            << ", цель: "
                            << car.get_target_speed()
                            << ", передача: "
                            << car.get_gear()
                            << '\n';
                    } else if (command == "exit") {
                        break;
                    } else {
                        throw std::invalid_argument(
                            "Неизвестная команда."
                        );
                    }
                }
            } catch (const std::exception& error) {
                std::cerr << error.what() << '\n';
            }
        }
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}