#pragma once
#include <string>
#include "Student.h"
#include "Tutor.h"


enum RequestStatus {
    PENDING,
    MATCHED,
    COMPLETED,
    CANCELLED
};
enum UrgencyLevel { 
    LOW = 0, 
    MEDIUM = 1, 
    HIGH = 2 };

class Request{
    protected:
    Student student;
    Tutor tutor;
    std::string subject;
    char description[300];
    UrgencyLevel urgency;
    RequestStatus status;
    bool is_accepted;


    public:
    Request();
    Request(Tutor t, Student s,std::string subject, RequestStatus status, UrgencyLevel urgency);
    //Request(Tutor t, Student s,std::string subject, char status, char urgency);
    Tutor get_tutor() const;
    Student get_student() const;
    RequestStatus get_status() const;
    UrgencyLevel get_urgency() const;
    std::string get_subject() const;
    bool get_is_accepted() const;
    char get_description() const; // update to char[]
    void update_status(RequestStatus updated_status);
    void update_is_accepted(bool accept);
};
