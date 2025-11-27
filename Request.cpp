#include <string>
#include "Request.h"
   
    Request::Request() = default;

    Request::Request(Tutor* t, Student* s,std::string subject, RequestStatus status, UrgencyLevel urgency,std::string description,const std::vector<bool>& days){
        tutor = t;
        student = s;
        this-> subject = subject;
        this-> status = status;
        this-> urgency = urgency;
        is_accepted = false;
        this-> description = description;
        this-> days = days;

    }
    //secondary constructor without defined tutor
    Request::Request(Student* s,std::string subject, UrgencyLevel urgency,std::string description,const std::vector<bool>& days){
        tutor = nullptr;    //null tutor
        student = s;
        this-> subject = subject;
        this-> status = POSTED;
        this-> urgency = urgency;
        is_accepted = false;
        this-> description = description;
        this-> days = days;
    }

    Tutor* Request::get_tutor() const{
        return tutor; 
    }
    Student* Request::get_student() const{
        return student;
    }
    Request::RequestStatus Request::get_status() const{
        return status;
    }
    Request::UrgencyLevel Request::get_urgency() const{
        return urgency;
    }
    std::string Request::get_subject(){
        return subject;
    }
    bool Request::get_is_accepted() const{
        return is_accepted;
    }
    std::string Request::get_description() const{
        return description;
    }
    std::vector<bool> Request::get_days() const{
        return days;
    }

    void Request::update_status(RequestStatus updated_status){
        status = updated_status;
    }
    void Request::update_is_accepted(bool accept){
        is_accepted = accept;
    }
    bool Request::match_tutor(Tutor* t){
        if(t != nullptr){
            tutor=t;
            return true;
        }
        else
            return false;
    }
