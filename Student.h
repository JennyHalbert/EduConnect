#pragma once
#include "User.h"
#include <string>
#include <vector>

class Request;
class Tutor;

class Student: public User{
private:
    std::vector<Request*> active_requests;
    std::vector<Request*> previous_requests;


public:
    Student();
    Student(std::string email,std::string name,std::string password);
    void add_request(Request*& r1);
    void close_request(Request* r1, int rating);
    std::vector<Request*> get_active_requests();
    std::vector<Request*> get_previous_requests();
};
