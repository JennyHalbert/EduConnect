#include <string>
#include "Request.h"
#include "Tutor.h"
#include "Student.h"
   
    Request::Request(Tutor t, Student s,std::string subject, char status, char urgency){
        this-> tutor = t;
        this-> student =s;
        this-> subject = subject;
        this-> status = status;
        this-> urgency = urgency;
        // this-> description = description;
        is_accepted = false;
    }
    Tutor Request::get_tutor(){
        return tutor;
    }
    Student Request::get_student(){
        return student;
    }
    char Request::get_status(){
        return status;
    }
    char Request::get_urgency(){
        return urgency;
    }
    bool Request::get_is_accepted(){
        return is_accepted;
    }
    // char Request::get_description(){ // update to char[]
    //     return description;
    // }
    void Request::update_status(char updated_status){
        status = updated_status;
    }
    void Request::update_is_accepted(bool accept){
        is_accepted = accept;
    }
