#include "pyramids_manager.h"

class console_interface {
private:
    pyramids_manager manager;

    bool try_input_positive_double(double& value);
    int input_positive_int();

    void input_pyramids(pyramids_manager& manager);
    void print_pyramid(const pyramid& object);

public:
    void run();
};
