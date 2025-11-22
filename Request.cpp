#include <string>
#include "Request.h"
#include "Tutor.h"
#include "Student.h"
   
    Request::Request() = default;

    Request::Request(Tutor t, Student s,std::string subject, RequestStatus status, UrgencyLevel urgency,std::string description,const std::vector<bool>& days){
        tutor = t;
        student = s;
        this-> subject = subject;
        this-> status = status;
        this-> urgency = urgency;
        is_accepted = false;
        this-> description = description;
        this-> days = days;

    }
    Tutor Request::get_tutor() const{
         return tutor; 
    }
    Student Request::get_student() const{
        return student;
    }
    RequestStatus Request::get_status() const{
        return status;
    }
    UrgencyLevel Request::get_urgency() const{
        return urgency;
    }
    std::string Request::get_subject() const{
        return subject;
    }
    bool Request::get_is_accepted() const{
        return is_accepted;
    }
    std::string Request::get_description() const{
        return description;
    }
    void Request::update_status(RequestStatus updated_status){
        status = updated_status;
    }
    void Request::update_is_accepted(bool accept){
        is_accepted = accept;
    }
