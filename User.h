#pragma once
#include <string>

class User{

protected:
    std::string email;
    std::string name;
    std::string password;

public:
    user(std::string email,std::string name,std::string password);
    std::string get_password();
    std::string get_email();
    std::string get_name();
};