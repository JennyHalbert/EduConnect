#include <string>
#include "User.h"
#include "Student.h"
#include "Request.h"
#include "Tutor.h"
#include <algorithm>
#include <iostream>

    Student::Student() = default;

    Student::Student(std::string email,std::string name,std::string password){
        this->email = email;
        this->name = name;
        this->password = password;
    }

    void Student::add_request(Request*& r1) {
        if (!r1) return;
    
        // db says this session is fully completed and already rated, request is no longer active
        if (r1->get_status() == Request::COMPLETED && r1->get_has_rated()) {
            previous_requests.push_back(r1);
        } else {
            active_requests.push_back(r1);
        }
    }

    void Student::close_request(Request* r1, int rating) {    
        if (!r1) {
            std::cout << "r1 is null\n";
            return;
        }
    
        Tutor* t = r1->get_tutor();
        if (!t) {
            std::cout << "Tutor pointer is null!\n";
        } else {
            t->update_ratings(rating);
            t->close_request(r1);
        }
    
        r1->update_status(Request::COMPLETED);
        r1->set_has_rated(true);
        r1->update_is_accepted(false); 
        previous_requests.push_back(r1);
    
        active_requests.erase(
            std::remove(active_requests.begin(), active_requests.end(), r1),
            active_requests.end()
        );

    }

    std::vector<Request*> Student::get_active_requests(){
        return active_requests;
    }
    std::vector<Request*> Student::get_previous_requests(){
        return previous_requests;
    }

   