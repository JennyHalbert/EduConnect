#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "Request.h"

class Tutor : public User{//this class is incomplete it is missing logic for the tutor constructor
    private:
        double total_ratings;
        int total_completed;
        int total_matched;
        std::vector<std::string> subjects;
        //request previous_requests[10];

    public:
        Tutor();
        Tutor(std::string email,std::string name,std::string password);
        double avg_rating();
        int avg_completion();
        int get_completed(); 
        void update_ratings(double rating);
        void update_completed();
        void update_matched();
};