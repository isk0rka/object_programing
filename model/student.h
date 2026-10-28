/* «Студент», который определяется полями ФИО, номер группы, название факультета, 
название специальности, средний балл успеваемости. */

#include <string>
#include <uuid/uuid.h>

class student
{
private:
    char id[37];

    std::string surname;
    std::string name;
    std::string Patronymic;
    
    int average_score;

    bool deleted;

    bool is_valid_fullname_part(std::string name_part);

    bool is_valid_fullname(std::string name, std::string surname, std::string Patronymic);

    bool set_id();

public:

    student();
    student(std::string name, std::string surname, std::string Patronymic);

    void set_delete();
    
    std::string get_full_name() const;
    int get_average_score() const;
    char get_id() const;

    ~student();
};