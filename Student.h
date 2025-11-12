#pragma once
#include "User.h"
<<<<<<< Updated upstream
#include "Request.h"

class Student: public User{//this class is incomplete, it is missing logic for the student constructor

private:
    request previous_requests[10];

public:
    student();
};
=======
#include <string>

class Student : public User {
public:
    Student();
    Student(std::string email, std::string name, std::string password);
};
>>>>>>> Stashed changes
