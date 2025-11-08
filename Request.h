#pragma once
#include "Student.h"
#include "Tutor.h"

class Request{
    private:
    Student student;
    Tutor tutor;
    std::string subject;
    char description[300];
    char urgency;
    char status;
    bool is_accepted;


};