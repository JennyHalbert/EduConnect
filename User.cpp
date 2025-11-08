#pragma once
#include <string>
#include "User.h"

    user::user(std::string email,std::string name,std::string password){
        user.email = email;
        user.name = name;
        user.password = password;
    }
    std::string  user::get_password(){
        return password;
    }
    std::string  user::get_email(){
        return email;
    }
    std::string  user::get_name(){
        return name;
    }