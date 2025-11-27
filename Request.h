#pragma once
#include <string>
#include <vector>

class Student;
class Tutor;
    
class Request{
    public:
    enum RequestStatus {POSTED, MATCHED, COMPLETED, CANCELLED};
    enum UrgencyLevel {LOW = 0, MEDIUM = 1, HIGH = 2};

    protected:
    Student* student;
    Tutor* tutor;
    std::string subject;
    std::string description;
    UrgencyLevel urgency;
    RequestStatus status;
    bool is_accepted;
    std::vector<bool> days;

    public:
    //Constructors
    Request();
    Request(Tutor* t, Student* s,std::string subject, RequestStatus status, UrgencyLevel urgency,std::string description,const std::vector<bool>& days);
    Request(Student* s,std::string subject, UrgencyLevel urgency,std::string description,const std::vector<bool>& days);//new

    //Functions
    Tutor* get_tutor() const;
    Student* get_student() const;
    RequestStatus get_status() const;
    UrgencyLevel get_urgency() const;
    std::string get_subject();
    bool get_is_accepted() const;
    std::string get_description() const;
    std::vector<bool>get_days()const;
    void update_status(RequestStatus updated_status);
    void update_is_accepted(bool accept);
    bool match_tutor(Tutor* t); //new


};

struct CompareRequestUrgency {//define a structure to handle the logic for the priority queue comparisons
    bool operator()(Request* a, Request* b) {
        // 'true' means 'a' has lower priority than 'b'
        return a->get_urgency() < b->get_urgency();
    }
};