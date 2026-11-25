#pragma once

class Car {
private:
    enum class Gear : int {
        Reverse = -1,
        Neutral = 0,
        First = 1,
        Second = 2,
        Third = 3,
        Fourth = 4,
        Fifth = 5
    };

    int speed = 0;
    int target_speed = 0;

    const int max_speed_forward;
    const int max_speed_back;

    Gear gear = Gear::Neutral;
    bool engine_work = false;

    static Gear gear_for_speed(int value) noexcept;
    void change_gear(Gear next_gear);

public:
    Car(int max_forward, int max_back);

    void set_engine_work(bool engine_run);
    void set_speed(int speed_);

    bool update();

    int get_speed() const noexcept;
    int get_target_speed() const noexcept;
    char get_gear() const noexcept;
    bool is_engine_working() const noexcept;
};