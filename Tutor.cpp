#include <string>
#include "User.h"
#include "Tutor.h"

    Tutor::Tutor(){
        
    }

    Tutor::Tutor(std::string email, std::string name, std::string password, std::vector<std::string> subjects)
    : User(email, name, password), subjects(subjects){ // can use std::move in this line to increase efficiency

    }

    void Tutor::setSubjects(const std::vector<std::string>& newSubjects){
        this -> subjects = newSubjects;
    }

    double Tutor::avg_rating() const{
        if (total_completed == 0) return 0.0;
        return total_rating/total_completed;
    }

    double Tutor::avg_completion() const{
        if (total_matched == 0) return 0.0;
        return total_completed/total_matched;
    }

    int Tutor::get_completed() const{
        return total_completed;
    }
