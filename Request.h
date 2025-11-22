#pragma once
#include <string>
#include "Student.h"
#include "Tutor.h"

enum RequestStatus {PENDING, MATCHED, COMPLETED, CANCELLED};
enum UrgencyLevel {LOW = 0, MEDIUM = 1, HIGH = 2};

class Request{
    protected:
    Student student;
    Tutor tutor;
    std::string subject;
    std::string description;
    UrgencyLevel urgency;
    RequestStatus status;
    bool is_accepted;
    std::vector<bool> days;


    public:
    //Constructors
    Request();
    Request(Tutor t, Student s,std::string subject, RequestStatus status, UrgencyLevel urgency,std::string description,const std::vector<bool>& days);
    //ENUMs to handle status and urgency
    //Functions
    Tutor get_tutor() const;
    Student get_student() const;
    RequestStatus get_status() const;
    UrgencyLevel get_urgency() const;
    std::string get_subject() const;
    bool get_is_accepted() const;
    std::string get_description() const;
    void update_status(RequestStatus updated_status);
    void update_is_accepted(bool accept);
};

struct CompareRequestUrgency {//define a structure to handle the logic for the priority queue comparisons
    bool operator()(Request* a, Request* b) {
        // 'true' means 'a' has lower priority than 'b'
        return a->get_urgency() < b->get_urgency();
    }
};