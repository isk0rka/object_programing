#include "university.h"
#include "../infrastructure/console.h"

#include <iostream>

int main() {
    university data;
    console interface(data, std::cin, std::cout);

    interface.run();
    return 0;
}
