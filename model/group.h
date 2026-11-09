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
    
    void set_list();

    void get_list() const;
    void get_student() const;

    ~group();
};