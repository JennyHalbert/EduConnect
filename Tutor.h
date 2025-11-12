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
        Request previous_requests[10];

    public:
        Tutor(); // default constructor

        Tutor(std:string email, std::string name, std::string password, std::vector<std::string> subjects = {}); // for registration

        void setSubjects(const std::vector<std::string>& newSubjects);

        double avg_rating() const;
        double avg_completion() const;
        int get_completed() const; 
};