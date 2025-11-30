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

    // void Student:: add_request(Request*& r1) {
    //     active_requests.push_back(r1);
    // }

    void Student::add_request(Request*& r1) {
        if (!r1) return;
    
        // If the DB says this session is fully completed AND already rated,
        // it belongs in history, not active.
        if (r1->get_status() == Request::COMPLETED && r1->get_has_rated()) {
            previous_requests.push_back(r1);
        } else {
            active_requests.push_back(r1);
        }
    }

    // void Student::close_request(Request* r1, int rating) {
    //     if (!r1) return;
    
    //     Tutor* t = r1->get_tutor();
    //     if (t) {
    //         t->update_ratings(rating);   // update stats
    //         t->close_request(r1);        // move into tutor.previous_requests
    //     }
    
    //     r1->update_status(Request::COMPLETED);
    //     r1->set_has_rated(true);
    
    //     // Move in student's lists
    //     previous_requests.push_back(r1);
    //     active_requests.erase(
    //         std::remove(active_requests.begin(), active_requests.end(), r1),
    //         active_requests.end()
    //     );

    void Student::close_request(Request* r1, int rating) {

        std::cout << "\n==============================\n";
        std::cout << "[DEBUG] Student::close_request START\n";
        std::cout << "Rating = " << rating << "\n";
    
        if (!r1) {
            std::cout << "[DEBUG] ERROR: r1 is NULL\n";
            std::cout << "==============================\n\n";
            return;
        }
    
        std::cout << "[DEBUG] Request pointer: " << r1 << "\n";
        std::cout << "[DEBUG] Request subject: " << r1->get_subject() << "\n";
        std::cout << "[DEBUG] Status BEFORE: " << r1->get_status() << "\n";
        std::cout << "[DEBUG] has_rated BEFORE: " << r1->get_has_rated() << "\n";
    
        // Check vectors before touching them
        std::cout << "[DEBUG] Student active_requests size BEFORE = "
                  << active_requests.size() << "\n";
        std::cout << "[DEBUG] Student previous_requests size BEFORE = "
                  << previous_requests.size() << "\n";
    
        // Show whether r1 is in active_requests before removing it
        bool found_in_active = false;
        for (auto* rq : active_requests) {
            if (rq == r1) { found_in_active = true; break; }
        }
        std::cout << "[DEBUG] r1 found in active_requests? "
                  << (found_in_active ? "YES" : "NO") << "\n";
    
        Tutor* t = r1->get_tutor();
        if (!t) {
            std::cout << "[DEBUG] WARNING: Tutor pointer is NULL!\n";
        } else {
            std::cout << "[DEBUG] Tutor email: " << t->get_email() << "\n";
            std::cout << "[DEBUG] Calling Tutor::update_ratings...\n";
            t->update_ratings(rating);
    
            std::cout << "[DEBUG] Calling Tutor::close_request...\n";
            t->close_request(r1);
        }
    
        std::cout << "[DEBUG] Updating request status to COMPLETED\n";
        r1->update_status(Request::COMPLETED);
    
        std::cout << "[DEBUG] Marking request has_rated=true\n";
        r1->set_has_rated(true);

        r1->update_is_accepted(false); 
    
        // Move in student's lists
        std::cout << "[DEBUG] Moving request to previous_requests\n";
        previous_requests.push_back(r1);
    
        std::cout << "[DEBUG] Removing request from active_requests\n";
        active_requests.erase(
            std::remove(active_requests.begin(), active_requests.end(), r1),
            active_requests.end()
        );
    
        // Show sizes after mutation
        std::cout << "[DEBUG] Student active_requests size AFTER = "
                  << active_requests.size() << "\n";
        std::cout << "[DEBUG] Student previous_requests size AFTER = "
                  << previous_requests.size() << "\n";
    
        std::cout << "[DEBUG] Status AFTER: " << r1->get_status() << "\n";
        std::cout << "[DEBUG] has_rated AFTER: " << r1->get_has_rated() << "\n";
    
        std::cout << "[DEBUG] Student::close_request END\n";
        std::cout << "==============================\n\n";
    }
    
    // }
    

    std::vector<Request*> Student::get_active_requests(){
        return active_requests;
    }
    std::vector<Request*> Student::get_previous_requests(){
        return previous_requests;
    }

   