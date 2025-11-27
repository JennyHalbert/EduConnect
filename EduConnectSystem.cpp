#include "EduConnectSystem.h"
#include <iostream>
#include "Algorithms.h"
//#include "sqlite3.h"
#include "Tutor.h"
#include "Student.h"
#include <vector>
#include <string>

EduConnectSystem::EduConnectSystem() {

}
EduConnectSystem::~EduConnectSystem() {
    for (auto& pair : Tutors) {
        delete pair.second;
    }
    Tutors.clear();

    // 2. Clean up Students
    for (auto& pair : Students) {
        delete pair.second; 
    }
    Students.clear();

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

bool EduConnectSystem::update_student_details(std::string current_email, std::string new_name, std::string new_email, std::string new_password){
    auto it = Students.find(current_email);
    if(it == Students.end()) 
        return false;

    Student* s = it->second;
    //handle email changes
    if(current_email != new_email){
        if(Students.count(new_email)){//check if email is used
            return false;
        }        
        Students.erase(current_email);//remove student from hashmap
        s->set_email(new_email);//update email
        Students[new_email]= s;//add back to hashmap
    }
        s->set_name(new_name);
        s->set_password(new_password);
        return true;
}

bool EduConnectSystem::update_tutor_details(std::string current_email,std::string new_name,std::string new_email,std::string new_pass,
    std::vector<std::string> new_subjects,std::vector<bool> new_days){
    auto it = Tutors.find(current_email);
    if(it == Tutors.end()){
        return false;
    }  

    Tutor* t = it->second;

    if(current_email != new_email){
        if(Tutors.count(new_email)){
            return false;
        }
        Tutors.erase(current_email);
        t->set_email(new_email);
        Tutors[new_email] = t;
    }
        t->set_name(new_name);
        t->set_password(new_pass);
        t->set_subjects(new_subjects);
        t->set_days(new_days);
        return true;
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
            Tutor* temp_login = get_tutor(email);
            if (temp_login ==nullptr){
                return false;
            }
            else if(temp_login->get_password()==password){
                return true;
            }
            else{
                return false;
            }
    }
    bool EduConnectSystem::student_login(std::string email,std::string password){
            Student* temp_login = get_student(email);
            if (temp_login ==nullptr){
                return false;
            }
            else if(temp_login->get_password()==password){
                return true;
            }
            else{
                return false;
            }
    }
    //Get information functions
    Tutor* EduConnectSystem::get_tutor(std::string email){
            auto it = Tutors.find(email);

            if(it != Tutors.end()){
                return it->second;
            }
            return nullptr;
    }
    Student* EduConnectSystem::get_student(std::string email){
            
            auto it = Students.find(email);
            if(it != Students.end()){
                return it->second;
            }
            return nullptr;
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

void EduConnectSystem::send_requests(Student* s,const std::vector<Tutor*>& selected_tutors, Request::UrgencyLevel urgency, 
                                     const std::string description,const std::string subject, const std::vector<bool>& days){

        Request* new_request = new Request(s,subject,urgency,description,days);
        for(Tutor* target : selected_tutors){
            target->receive_request(new_request);
        }
        s->add_request(new_request);
}
