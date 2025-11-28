#include <string>
#include "User.h"
#include "Student.h"
#include "Request.h"
#include "Tutor.h"
#include <algorithm>

    Student::Student() = default;

    Student::Student(std::string email,std::string name,std::string password){
        this->email = email;
        this->name = name;
        this->password = password;
    }

    void Student:: add_request(Request*& r1) {
        active_requests.push_back(r1);
    }


    ///////// added in 
    // Used when rebuilding state from the database so we don't re-apply ratings.
    void Student::restore_request(Request* r1) {
        if (!r1) return;
        if (r1->get_status() == Request::COMPLETED || r1->get_status() == Request::CANCELLED) {
            previous_requests.push_back(r1);
            return;
        }
        active_requests.push_back(r1);
    }
    ///////// added in 

    
    //removes reqeuest from the active requests and adds it to previous requests
    void Student::close_request(Request* r1,int rating){
        r1->get_tutor()->update_ratings(rating);
        r1->update_status(Request::COMPLETED);
        previous_requests.push_back(r1);
        active_requests.erase(std::remove(active_requests.begin(),active_requests.end(),r1),active_requests.end());
    }

    std::vector<Request*> Student::get_active_requests(){
        return active_requests;
    }
    std::vector<Request*> Student::get_previous_requests(){
        return previous_requests;
    }
