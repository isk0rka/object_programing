/* «Студент», который определяется полями ФИО, номер группы, название факультета, 
название специальности, средний балл успеваемости. */

#include <string>

class student
{
private:
    int id;
    std::string name;
    bool deleted;
public:
    student();
    ~student();
};