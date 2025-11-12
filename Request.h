#pragma once
#include "Student.h"
#include "Tutor.h"


enum class RequestStatus {
    PENDING,
    MATCHED,
    COMPLETED,
    CANCELLED
};

class Request{
    protected:
    Student student;
    Tutor tutor;
    std::string subject;
    char description[300];
    char urgency;
    char status;
    bool is_accepted;

    public:
    Request();
    Request(Tutor t, Student s,std::string subject, char status, char urgency);
    Tutor get_tutor();
    Student get_student();
    char get_status();
    char get_urgency();
    bool get_is_accepted();
    char get_description(); // update to char[]
    void update_status(char updated_status);
    void update_is_accepted(bool accept);
};