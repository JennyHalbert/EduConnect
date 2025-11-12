#pragma once
#include "User.h"
#include "Request.h"
#include <string>


class Student: public User{

private:
    Request previous_requests[10];

public:


    Student();
    Student(std::string email,std::string name,std::string password);
};
