#pragma once
#include <string>
#include "User.h"
#include <queue>
#include <vector>
#include "Request.h"

class Tutor: public User{
    private:
        double total_ratings;
        int total_completed;
        int total_matched;
        std::vector<std::string> subjects;
        std::vector<bool> days;
        std::vector<Request*> active_requests;
        std::vector<Request*> previous_requests;
        std::priority_queue<Request*,std::vector<Request*>,CompareRequestUrgency> request_inbox;

    public:
        Tutor();
        Tutor(std::string email,std::string name,std::string password,const std::vector<bool>& days,std::vector<std::string> subjects);

        void set_subjects(const std::vector<std::string>& newSubjects);
        void set_days(const std::vector<bool>& days);

        double avg_rating();
        double avg_completion();
        int get_completed() const;
        int get_matched() const;
        std::vector<bool> get_days();
        std::vector<std::string> get_subjects();
        std::vector<Request*> get_active_requests();
        std::vector<Request*> get_previous_requests();

        void update_ratings(double rating);
        void update_matched();

        void receive_request(Request* r);
        bool accept_request(Request* r);
        void close_request(Request*r);
        void set_totals(double totalRatings, int completed, int matched);
        double get_total_ratings() const { return total_ratings; }

        void clean_inbox();
        std::vector<Request*> get_valid_inbox();
        bool is_available(int dayIndex) const;
};
