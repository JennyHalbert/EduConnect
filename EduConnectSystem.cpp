#include "EduConnectSystem.h"
#include <iostream>
#include "Algorithms.h" // Your Merge Sort template
#include "Tutor.h"
#include <vector>
#include <string>

EduConnectSystem::EduConnectSystem() {

}
EduConnectSystem::~EduConnectSystem() {
// 1. Clean up Tutors
    for (auto& pair : Tutors) {
        delete pair.second; // Delete the heap pointer
    }
    Tutors.clear();

    // 2. Clean up Students
    for (auto& pair : Students) {
        delete pair.second; // Delete the heap pointer
    }
    Students.clear();

    // 3. Clean up Index maps
    // (We don't delete pointers here because we just did it above)
    tutors_by_subject.clear();
}
void EduConnectSystem::index_tutor(Tutor* t, const std::vector<std::string>& subjects){
    for(const std::string& sub:subjects){
        tutors_by_subject[sub].push_back(t);
    }
}

void EduConnectSystem::update_tutor_subjects(Tutor* tutor, const std::vector<std::string>& new_subjects) {

    if(tutor==nullptr) return;

    std::vector<std::string> old_subjects = tutor->get_subjects();

    for(const std::string& old_sub : old_subjects){
        if(tutors_by_subject.count(old_sub)){
            
            std::vector<Tutor*>& tutor_list = tutors_by_subject[old_sub];

            tutor_list.erase(std::remove(tutor_list.begin(),tutor_list.end(),tutor),tutor_list.end());
        }
    }
    tutor->set_subjects(new_subjects);
    
    for(const std::string& new_sub: new_subjects){
        tutors_by_subject[new_sub].push_back(tutor);
    }
}

    //Register Functions
    bool EduConnectSystem::register_tutor(std::string name, std::string email,std::string password,const std::vector<bool>& days, std::vector<std::string> subjects){
        if(Tutors.count(email)){
         return false;
        }
        Tutor* new_Tutor = new Tutor(email, name, password, days, subjects);
        Tutors[email] = new_Tutor;
        index_tutor(new_Tutor,subjects);
        return true;


    }
    bool EduConnectSystem::register_student(std::string name,std::string email,std::string password){
        if(Students.count(email)){
         return false;
        }
        Student* new_Student = nullptr;
        new_Student = new Student(email, name, password);
        Students[email] = new_Student;
        return true;
    }
    //Login Functions
    bool EduConnectSystem::tutor_login(std::string email,std::string password){
            Tutor* temp_login = Tutors[email];
            if(temp_login->get_password()==password){
                return true;
            }
            else{
                return false;
            }
    }
    bool EduConnectSystem::student_login(std::string email,std::string password){
            Student* temp_login = Students[email];
            if(temp_login->get_password()==password){
                return true;
            }
            else{
                return false;
            }
    }
    //Get information functions
    Tutor* EduConnectSystem::get_tutor(std::string email){
            return Tutors[email];
    }
    Student* EduConnectSystem::get_student(std::string email){
            return Students[email];
    }


    std::vector<Tutor*> EduConnectSystem::get_tutors_for_subject(std::string subject, std::string sort_criteria, const std::vector<bool> days){
        std::vector<Tutor*> results;
    
        const std::vector<Tutor*>& candidates = tutors_by_subject[subject];

        for(Tutor* t : candidates){
            bool match = false;

            for(int i = 0; i<7;i++){             
                if(days[i] && t->is_available(i)){
                    match = true;
                    break;
                }
            }
            if(match){
                results.push_back(t);
            }
        }
        
        if(sort_criteria=="RATING"){
            merge_sort(results,0, results.size()-1,[](Tutor*a,Tutor* b){
                return a->avg_rating()>= b->avg_rating();
            });
        }
        else if (sort_criteria == "EXPERIENCE") {
        merge_sort(results, 0, results.size() - 1, [](Tutor* a, Tutor* b) {
            return a->get_completed() >= b->get_completed();
        });
        }
        else if (sort_criteria == "COMPLETIONRATE") {
        merge_sort(results, 0, results.size() - 1, [](Tutor* a, Tutor* b) {
            return a->avg_completion() >= b->avg_completion();
        });
        }  
        else
        merge_sort(results, 0, results.size() - 1, [](Tutor* a, Tutor* b) {
            return a->get_name() >= b->get_name();
        });
        return results;
}