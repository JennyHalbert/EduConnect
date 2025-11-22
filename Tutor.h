#pragma once
#include <string>
#include <vector>
#include "User.h"
#include <queue>
#include <vector>

class Tutor : public User{//this class is incomplete it is missing logic for the tutor constructor
    private:
        double total_ratings;
        int total_completed;
        int total_matched;
        std::vector<std::string> subjects;
        std::vector<Request*> active_requests;
        std::priority_queue<Request*,std::vector<Request*>,CompareRequestUrgency> request_inbox;
        std::vector<bool> days;
        // Create a priority queue of request pointers, define how the priority queue stores data, and define how the priority queue pointers will be compared

    public:
        Tutor();
        // Tutor(std:string email, std::string name, std::string password, std::vector<std::string> subjects = {}); // for registration
        Tutor(std::string email,std::string name,std::string password,const std::vector<bool>& days);
        double avg_rating();
        int avg_completion();
        int get_completed() const; 
        void update_ratings(double rating);
        void update_completed();
        void update_matched();
        void setSubjects(const std::vector<std::string>& newSubjects);
        void clean_inbox();
        Request* next_request();
};
