#include <string>
#include <uuid/uuid.h>

#include "student.h"
#include "CONSTANTS.h"

bool student::is_valid_fullname_part(std::string name_part){
    return ((!name_part.empty())
             && (name_part.find_first_of(FORBIDDEN_FULLNAME_CHARS)
             == std::string::npos)) ;
}

bool student::is_valid_fullname(std::string name, std::string surname, std::string Patronymic){
    return (is_valid_fullname_part(name) 
            && is_valid_fullname_part(surname)
            && is_valid_fullname_part(Patronymic));
}

bool student::set_id(){
    uuid_t binuuid;
    uuid_generate(binuuid);

    uuid_unparse(binuuid, id);
}

student::student(){
    surname = "Иванов";
    name = "Иван";
    Patronymic = "Иванович";
}

student::student(std::string name, std::string surname, std::string Patronymic){
    if (!is_valid_fullname(name, surname, patronymic)) {
        throw std::invalid_argument("Invalid full name");
    } else {
        this->name = name;
        this->surname = surname;
        this->Patronymic = Patronymic;
    }
}

void student::set_delete(){
    if (!deleted){
        deleted = true ;
    } else { throw ;}
}

std::string student::get_full_name() const{
    return name + " " +
        surname + " " +
        Patronymic;
}

int student::get_average_score() const{
    return average_score;
}

int student::get_id() const{
    return id;
}

student::~student(){

}