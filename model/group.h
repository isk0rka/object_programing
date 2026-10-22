#include <string>
#include <vector>

#include "student.h"

class group
{
private:

    std::string name;
    std::vector<student> list; 

    bool deleted;
public:
    group();



    ~group();
};