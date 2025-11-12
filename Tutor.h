#pragma once
#include <string>
#include <vector>
#include "User.h"
#include "Request.h"

class Tutor : public User{//this class is incomplete it is missing logic for the tutor constructor
    private:
        double total_ratings = 0.0;
        int total_completed = 0;
        int total_matched = 0;
        std::vector<std::string> subjects;
        //request previous_requests[10];

    public:
        Tutor();
        // Tutor(std:string email, std::string name, std::string password, std::vector<std::string> subjects = {}); // for registration
        Tutor(std::string email,std::string name,std::string password);
        double avg_rating();
        int avg_completion();
        int get_completed(); 
        void update_ratings(double rating);
        void update_completed();
        void update_matched();
        void setSubjects(const std::vector<std::string>& newSubjects);

};