#pragma once
#include <string>

class User{

protected:
    std::string email;
    std::string name;
    std::string password;

public:
    User();
    User(std::string email,std::string name,std::string password);
    std::string get_password() const;
    std::string get_email() const;
    std::string get_name() const;
};

// //Stores student/tutor objects
// extern std::unordered_map<std::string, std::unique_ptr<User>> userDatabase;

// // Insertion for user registration
// void registerNewUser(std::unique_ptr<User> newUser);

// //Retrieval
// User* getUserProfile(const std::string& email);

// // Update tutor subjects
// // void updateTutorSubjects(const std::string& email, const std::vector<std::string>& newSubjects);