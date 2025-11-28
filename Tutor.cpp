#include <string>
#include "User.h"
#include "Tutor.h"
#include "Request.h"

// Tutor::Tutor(std::string email, std::string name, std::string password, std::vector<std::string> subjects)
// : User(email, name, password), subjects(subjects){ // can use std::move in this line to increase efficiency

// }
    Tutor::Tutor() = default;

    Tutor::Tutor(std::string email,std::string name,std::string password,const std::vector<bool>& days, std::vector<std::string> subjects)
    : User(email, name, password),
      total_ratings(0.0),
      total_completed(0),
      total_matched(0),
      days(days),
      subjects(subjects){
    }
    // added
    void Tutor::set_subjects(const std::vector<std::string>& newSubjects){
        subjects = newSubjects;
    }
    void Tutor::set_days(const std::vector<bool>& days){
        this->days = days;
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

    std::vector<std::string> Tutor::get_subjects(){
        return subjects;
    }

// std::vector<Request*> Tutor::get_active_requests(){
//         std::vector<Request*> temp_list;
//         Request* temp;
//         for(int i = 0; i<active_requests.size();i++){
//             temp = active_requests[i];
//             if(temp->get_status()== Request::COMPLETED){
//                 active_requests.pop_back();
//                 previous_requests.push_back(temp);
//             }
//             else{
//                 active_requests.pop_back();
//                 temp_list.push_back(temp);
//             }
//         }
//         for(Request* r: temp_list){
//             active_requests.push_back(r);
//         }
//         return active_requests;

// }

// Tutor.cpp

std::vector<Request*> Tutor::get_active_requests() {
    std::vector<Request*> still_active;
    
    // Iterate through the current active_requests
    for (Request* r : active_requests) {
        if (r->get_status() == Request::COMPLETED) {
            // Move to previous history
            previous_requests.push_back(r);
        } else {
            // Keep in active
            still_active.push_back(r);
        }
    }

    // Replace the old list with the filtered list
    active_requests = still_active;
    
    return active_requests;
}

    int Tutor::get_completed() const{
        return total_completed;
    }
    int Tutor::get_matched() const{
        return total_matched;
    }
    std::vector<bool> Tutor::get_days(){
        return days;
    }


    void Tutor::update_ratings(double rating){
        total_ratings+=rating;
        total_completed++;
    }
    void Tutor::update_matched(){
        total_matched++;
    }
    void Tutor::clean_inbox(){
        while(!request_inbox.empty()){

            Request* top_req = request_inbox.top();
            if(top_req->get_status()!= Request::POSTED){
                request_inbox.pop();
            }
            else{
            break;
            }
        }
    }
    std::vector<Request*> Tutor::get_valid_inbox(){
        std::vector<Request*> display_list;
        std::vector<Request*> temp_storage;

        while(!request_inbox.empty()){  //Empty the priority queue inbox
            Request* req = request_inbox.top();
            request_inbox.pop();

            if(req->get_status()== Request::POSTED){
                display_list.push_back(req);
                temp_storage.push_back(req);
            }
        }
        
        for(Request* req : temp_storage){
            request_inbox.push(req);
        }
        return display_list;
    }
    std::vector<Request*> Tutor::get_previous_requests(){
        return previous_requests;
    }


    bool Tutor::is_available(int dayIndex) const{
        if (dayIndex >= 0 && dayIndex < 7) {
            return days[dayIndex];
        }
        return false;
    }

    void Tutor::receive_request(Request* r){
        if(r==nullptr) return;
        request_inbox.push(r);
    }

    bool Tutor::accept_request(Request* r){
        if(r->get_status()!= Request::POSTED){
            return false;
        }
        r->update_status(Request::MATCHED);
        r->match_tutor(this);
        active_requests.push_back(r);
        total_matched++;
        return true;
    }
    void Tutor::close_request(Request*r){
        if(r->get_status()!= Request::MATCHED){
            return;
        }
        r->update_status(Request::COMPLETED);
        previous_requests.push_back(r);
    }


