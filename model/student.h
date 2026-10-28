/* «Студент», который определяется полями ФИО, номер группы, название факультета, 
название специальности, средний балл успеваемости. */

#include <string>

class student
{
private:
    char[37] id

    std::string surname;
    std::string name;
    std::string Patronymic;
    
    bool deleted;

    bool is_valid_fullname_part(std::string name_part);

    bool is_valid_fullname(std::string name, std::string surname, std::string Patronymic);

public:

    student();
    student(std::string name, std::string surname, std::string Patronymic);

    void set_delete();
    
    std::string get_full_name() const;
    int get_id() const;

    ~student();
};