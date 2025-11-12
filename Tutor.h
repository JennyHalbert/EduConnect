#pragma once
#include <string>
#include <vector>
#include "User.h"

class Tutor : public User{//this class is incomplete it is missing logic for the tutor constructor
    private:
        double total_ratings;
        int total_completed;
        int total_matched;
        std::vector<std::string> subjects;
        request previous_requests[10];

    public:
        tutor();
        double avg_rating();
        int avg_completion();
<<<<<<< Updated upstream
        int get_completed(); 
};
=======
        int get_completed() const;
        void update_ratings(double rating);
        void update_completed();
        void update_matched();
        void setSubjects(const std::vector<std::string>& newSubjects);

};
>>>>>>> Stashed changes
