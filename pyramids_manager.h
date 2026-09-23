#include <vector>
#include "pyramid.h"

class pyramids_manager {
private:
    std::vector<pyramid> pyramids;

public:
    void add_pyramid(double side, double height);

    const pyramid* get_max_base_area() const;
    const pyramid* get_max_volume() const;
    
    const pyramid* get_pyramid(size_t index) const;
    
    int get_count() const;
};