#include <string>
#include "User.h"
#include "Student.h"

    Student::Student(std::string email,std::string name,std::string password){
        this->email = email;
        this->name = name;
        this->password = password;
    }