#include "university.h"
#include "../infrastructure/console.h"

#include <exception>
#include <iostream>

int main() {
    try {
        university data;
        console interface(data, std::cin, std::cout);

        interface.run();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Критическая ошибка: " << error.what() << '\n';
        return 1;
    }
}
