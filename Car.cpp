#include "Car.h"
#include <stdexcept>

Car::Car(int max_forward, int max_back)
    : max_speed_forward(max_forward),
      max_speed_back(max_back) {

    if (max_forward <= 0 || max_back <= 0) {
        throw std::invalid_argument(
            "Максимальные скорости должны быть положительными."
        );
    }
}

void Car::set_engine_work(bool engine_run) {
    if (engine_work == engine_run) {
        throw std::logic_error(
            "Двигатель уже находится в указанном состоянии."
        );
    }

    if (!engine_run) {
        if (speed != 0) {
            throw std::logic_error(
                "Невозможно выключить двигатель во время движения."
            );
        }

        if (target_speed != 0) {
            throw std::logic_error(
                "Необходимо установить целевую скорость 0."
            );
        }

        if (gear != Gear::Neutral) {
            throw std::logic_error(
                "Коробка передач не находится в нейтральном положении."
            );
        }
    }

    engine_work = engine_run;
}

void Car::set_speed(int speed_) {
    if (!engine_work) {
        throw std::logic_error(
            "Двигатель выключен."
        );
    }

    if (speed_ > max_speed_forward ||
        speed_ < -max_speed_back) {
        throw std::out_of_range(
            "Целевая скорость превышает допустимые пределы."
        );
    }

    target_speed = speed_;
}

Car::Gear Car::gear_for_speed(int value) noexcept {
    if (value < 0)
        return Gear::Reverse;

    if (value == 0)
        return Gear::Neutral;

    if (value <= 20)
        return Gear::First;

    if (value <= 40)
        return Gear::Second;

    if (value <= 70)
        return Gear::Third;

    if (value <= 110)
        return Gear::Fourth;

    return Gear::Fifth;
}

void Car::change_gear(Gear next_gear) {
    const int difference =
        static_cast<int>(next_gear) -
        static_cast<int>(gear);

    if (difference < -1 || difference > 1) {
        throw std::logic_error(
            "Запрещено пропускать передачи."
        );
    }

    gear = next_gear;
}

bool Car::update() {
    if (!engine_work || speed == target_speed) {
        return false;
    }

    const int next_speed =
        speed + (speed < target_speed ? 1 : -1);

    change_gear(gear_for_speed(next_speed));

    speed = next_speed;

    return true;
}

int Car::get_speed() const noexcept {
    return speed;
}

int Car::get_target_speed() const noexcept {
    return target_speed;
}

char Car::get_gear() const noexcept {
    if (gear == Gear::Reverse)
        return 'R';

    if (gear == Gear::Neutral)
        return 'N';

    return static_cast<char>(
        '0' + static_cast<int>(gear)
    );
}

bool Car::is_engine_working() const noexcept {
    return engine_work;
}