#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "sqlite3.h"
#include "User.h"
#include "Student.h"
#include "Tutor.h"
#include "Request.h"
#include "Database.h"

class EduConnectSystem {
public:
    EduConnectSystem();  // Constructor loads data from SQL
    ~EduConnectSystem(); // Destructor closes SQL
    void index_tutor(Tutor* t, const std::vector<std::string>& subjects);//adds tutors to subject vector hashmap

    // --- GUI calls these functions ---    
    // User Management
    void update_tutor_subjects(Tutor* tutor, const std::vector<std::string>& new_subjects);
    bool update_student_details(std::string currentEmail, std::string newName, std::string newEmail, std::string newPassword);
    bool update_tutor_details(std::string current_email,std::string new_name,std::string new_email,std::string new_pass,
                              std::vector<std::string> new_subjects,std::vector<bool> new_days);
    //Register
    bool register_tutor(std::string name, std::string email,std::string password,const std::vector<bool>& days, std::vector<std::string> subjects);
    bool register_student(std::string name,std::string email,std::string password);
    //Login
    bool tutor_login(std::string email,std::string password);
    bool student_login(std::string email,std::string password);
    //Get Info
    Tutor* get_tutor(std::string email);
    Student* get_student(std::string email);

    // Returns a sorted list of tutors for the GUI to display in a list box
    std::vector<Tutor*> get_tutors_for_subject(std::string subject, std::string sort_criteria,const std::vector<bool> days);

    void send_requests(Student* s,const std::vector<Tutor*>& selected_tutors, Request::UrgencyLevel urgency,
        const std::string description, std::string subject, const std::vector<bool>& days);

    bool accept_request(Tutor* t, Request* r);
    bool complete_request(Request* r);

    // Requests

private:
    // --- Data Structures ---
    std::unordered_map<std::string, Tutor*> Tutors; 
    std::unordered_map<std::string, Student*> Students; 
    std::unordered_map<std::string, std::vector<Tutor*>> tutors_by_subject;
    std::vector<Request*> allRequests;
    
    // --- Database ---
    Database db_{"educonnect.db"};
    
    // Student* loadStudentFromDB(const std::string& email);
    // Tutor*   loadTutorFromDB(const std::string& email);

    // bool openDB(const std::string& filename);
    // void closeDB();
    // bool initSchema();
    // bool executeSQL(const char* sql);



    // // --- Database ---
    //sqlite3* db;    
    // //Helper to run internal SQL commands
    // void executeSQL(const char* sql);
    // void loadUsersFromDB();
};
