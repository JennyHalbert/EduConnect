#pragma once
#include <string>

class User{

protected:
    std::string email;
    std::string name;
    std::string password;
    int account_ID;

public:
    User();
    User(std::string email,std::string name,std::string password);
    std::string get_password() const;
    std::string get_email() const;
    std::string get_name() const;
    int get_ID() const;

    void set_email(std::string new_email);
    void set_name(std::string new_name);
    void set_password(std::string new_password);
};
