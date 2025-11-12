#include <string>
#include "User.h"
#include "Tutor.h"

    Tutor::Tutor(std::string email,std::string name,std::string password){
        this->email = email;
        this->name = name;
        this->password = password;
        this->total_ratings = 0;
        this->total_completed = 0;
        this->total_matched = 0;
    }

    double Tutor::avg_rating(){
        if(total_completed==0){
            return 0.0;
        }
        else{
        return total_ratings/total_completed;
        }
    }

    int Tutor::avg_completion(){
        if(total_matched==0){
            return 0;
        }
        else{
        return total_completed/total_matched;
        }
    }

    int Tutor::get_completed(){
        return total_completed;
    }
    void Tutor::update_ratings(double rating){
        total_ratings+=rating;
    }
    void Tutor::update_completed(){
        total_completed++;
    }
    void Tutor::update_matched(){
        total_matched++;
    }