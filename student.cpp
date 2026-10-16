#include <string>

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

student::student(){

}
student::student(std::string name, std::string surname, std::string Patronymic){

}

void student::set_delete(){
    if (!deleted){
        deleted = true ;
    } else { throw ;}
}

void student::get_full_name() const{

}

student::~student(){

}