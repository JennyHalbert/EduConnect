#include <string>
#include "User.h"
#include "Tutor.h"
#include "Request.h"
#include <iostream>

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

    double Tutor::avg_completion(){
        std ::cout << "avg_completion is called \n"; 

        if(total_matched==0){
            std ::cout << "into if avg_completion \n"; 

            return 0.0;
        }
        else{
            double value = static_cast<double>(total_completed) /
                       static_cast<double>(total_matched);

            std ::cout << "into else avg_completion total_completed "<< total_completed<<" and total_matched "<<total_matched<<" total "<<total_completed/total_matched; 
            return value;
            //return total_completed/total_matched;
        }
    }

    std::vector<std::string> Tutor::get_subjects(){
        return subjects;
    }

void Tutor::set_totals(double totalRatings, int completed, int matched) {
    total_ratings  = totalRatings;
    total_completed = completed;
    total_matched   = matched;
}

std::vector<Request*> Tutor::get_active_requests() {
    //  COMPLETED requests move from active_requests to previous_requests
    auto it = active_requests.begin();
    while (it != active_requests.end()) {
        Request* r = *it;
        if (r && r->get_status() == Request::COMPLETED) {
            previous_requests.push_back(r);
            it = active_requests.erase(it);   // erase returns the next iterator
        } else {
            ++it;
        }
    }
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

        while(!request_inbox.empty()){  
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


