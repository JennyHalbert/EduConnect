#include <string>
#include "User.h"

    User::User() = default;

    User::User(std::string email,std::string name,std::string password){
        this->email = email;
        this->name = name;
        this->password = password;
    }
    std::string User::get_password() const{
        return password;
    }
    std::string User::get_email() const{
        return email;
    }
    std::string User::get_name() const{
        return name;
    }
    int User::get_ID() const{
        return account_ID;
    }

    void User::set_email(std::string new_email){
        email=new_email;
    }
    void User::set_name(std::string new_name){
        name=new_name;
    }
    void User::set_password(std::string new_password){
        password=new_password;
    }

    