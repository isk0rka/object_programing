/* «Студент», который определяется полями ФИО, номер группы, название факультета, 
название специальности, средний балл успеваемости. */

#include <string>

class student
{
private:
    int id;
    std::string name;
    std::string surname;
    std::string Patronymic;
    bool deleted;
public:
    student();
    ~student();
};