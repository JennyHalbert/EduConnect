#include <string>
#include "User.h"
#include "Tutor.h"

    tutor::tutor(){
        
    }

    double Tutor::avg_rating(){
        return total_rating/total_completed;
    }

    int Tutor::avg_completion(){
        return total_completed/total_matched;
    }

    int Tutor::get_completed(){
        return total_completed;
    }
