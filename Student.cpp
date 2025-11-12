<<<<<<< Updated upstream
=======
#include <string>
#include <utility>
#include "User.h"
#include "Student.h"

Student::Student() = default;

Student::Student(std::string email, std::string name, std::string password)
    : User(std::move(email), std::move(name), std::move(password)) {}
>>>>>>> Stashed changes
