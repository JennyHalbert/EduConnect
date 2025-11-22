#include <string>
#include "User.h"
#include "Student.h"

    Student::Student() = default;

    Student::Student(std::string email,std::string name,std::string password){
        this->email = email;
        this->name = name;
        this->password = password;
    }

    void Student:: addrequest(Request*& r1) {
        previous_requests.push_back(r1);
    }
