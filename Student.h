#pragma once
#include "User.h"
#include "Request.h"

class Student: public User{//this class is incomplete, it is missing logic for the student constructor

private:
    request previous_requests[10];

public:
    student();
};