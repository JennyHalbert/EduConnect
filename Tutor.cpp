#include <string>
#include <utility>
#include "User.h"
#include "Tutor.h"

<<<<<<< Updated upstream
    tutor::tutor(){
        
=======

// Tutor::Tutor(std::string email, std::string name, std::string password, std::vector<std::string> subjects)
// : User(email, name, password), subjects(subjects){ // can use std::move in this line to increase efficiency

// }
Tutor::Tutor() = default;

Tutor::Tutor(std::string email, std::string name, std::string password)
    : User(std::move(email), std::move(name), std::move(password)),
      total_ratings(0.0),
      total_completed(0),
      total_matched(0) {}

    // added
    void Tutor::setSubjects(const std::vector<std::string>& newSubjects){
        this -> subjects = newSubjects;
>>>>>>> Stashed changes
    }

    double Tutor::avg_rating(){
        return total_rating/total_completed;
    }

    int Tutor::avg_completion(){
        return total_completed/total_matched;
    }

<<<<<<< Updated upstream
    int Tutor::get_completed(){
        return total_completed;
=======
int Tutor::get_completed() const{
    return total_completed;
}
    void Tutor::update_ratings(double rating){
        total_ratings+=rating;
    }
    void Tutor::update_completed(){
        total_completed++;
    }
    void Tutor::update_matched(){
        total_matched++;
>>>>>>> Stashed changes
    }
