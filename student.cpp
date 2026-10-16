#include <string>

#include "student.h"
#include "CONSTANTS.h"

bool student::is_valid_fullname_part(std::string name_part){
    for (size_t i = 0; i < name_part.size(); i++){
        if (!(INCORECT_CHAR_FULLNAME.find()==)){
            return 
        }
    }
}

bool student::is_valid_fullname(std::string name, std::string surname, std::string Patronymic){


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