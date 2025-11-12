#include <string>
#include <utility>
#include "User.h"

    user::user(std::string email,std::string name,std::string password){
        user.email = email;
        user.name = name;
        user.password = password;
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
    