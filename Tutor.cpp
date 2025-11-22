#include <string>
#include "User.h"
#include "Tutor.h"
#include "Request.h"


// Tutor::Tutor(std::string email, std::string name, std::string password, std::vector<std::string> subjects)
// : User(email, name, password), subjects(subjects){ // can use std::move in this line to increase efficiency

// }
    Tutor::Tutor() = default;

    Tutor::Tutor(std::string email,std::string name,std::string password,const std::vector<bool>& days)
    : User(email, name, password),
      total_ratings(0.0),
      total_completed(0),
      total_matched(0),
      days(days) {
    }

    // added
    void Tutor::setSubjects(const std::vector<std::string>& newSubjects){
        this -> subjects = newSubjects;
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

    int Tutor::get_completed() const{
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
    void Tutor::clean_inbox(){
        while(!request_inbox.empty()){

            Request* top_req = request_inbox.top();
            if(top_req->get_status()!= Request::PENDING){
                request_inbox.pop();
            }
            else{
            break;
            }
        }
    }
    Request* Tutor::next_request(){
        clean_inbox();
        if(request_inbox.empty()){
            return nullptr;
        }
        return request_inbox.top();

    }
