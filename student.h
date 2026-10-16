/* «Студент», который определяется полями ФИО, номер группы, название факультета, 
название специальности, средний балл успеваемости. */

#include <string>

class student
{
private:

    std::string name;
    std::string surname;
    std::string Patronymic;
    
    bool deleted;

    bool is_valid_name(std::string name);
    bool is_valid_surname(std::string surname);
    bool is_valid_Patronymic(std::string Patronymic);

    bool is_valid_fullname();

public:

    student();
    student(std::string name, std::string surname, std::string Patronymic);

    void set_delete();
    
    void get_full_name() const;

    ~student();
};