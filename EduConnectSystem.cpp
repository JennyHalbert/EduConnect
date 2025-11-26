#include "EduConnectSystem.h"
#include <iostream>
#include "Algorithms.h" 
#include "Tutor.h"
#include <vector>
#include <string>

EduConnectSystem::EduConnectSystem() {

}
EduConnectSystem::~EduConnectSystem() {
    std::cout << "[EduConnectSystem] Destructor: cleaning up...\n";
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

    closeDB();
    std::cout << "[EduConnectSystem] Destructor finished.\n";
}


// add db stuff in 
// DB 
// ---------- SQLite helpers ----------

bool EduConnectSystem::openDB(const std::string& filename) {
    // If it's already open, don't reopen
    if (db) {
        std::cout << "[DB] openDB: DB already open.\n";
        return true;
    }

    std::cout << "[DB] Opening database: " << filename << "\n";

    int rc = sqlite3_open(filename.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "[DB] ERROR opening database: " << sqlite3_errmsg(db) << "\n";
        db = nullptr;
        return false;
    }

    std::cout << "[DB] Opened successfully.\n";

    // 🔹 ENSURE SCHEMA EXISTS *RIGHT HERE*
    const char* schema = R"SQL(
        CREATE TABLE IF NOT EXISTS students (
            id       INTEGER PRIMARY KEY AUTOINCREMENT,
            name     TEXT NOT NULL,
            email    TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );

        CREATE TABLE IF NOT EXISTS tutors (
            id       INTEGER PRIMARY KEY AUTOINCREMENT,
            name     TEXT NOT NULL,
            email    TEXT NOT NULL UNIQUE,
            password TEXT NOT NULL
        );
    )SQL";

    std::cout << "[DB] Ensuring schema exists inside openDB()...\n";
    if (!executeSQL(schema)) {
        std::cerr << "[DB] ERROR creating schema inside openDB().\n";
        // We still keep DB open so the program can run using in-memory structures.
    } else {
        std::cout << "[DB] Schema ready (students / tutors tables).\n";
    }

    return true;
}

void EduConnectSystem::closeDB() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool EduConnectSystem::executeSQL(const char* sql) {
    std::cout << "[DB] Executing SQL:\n" << sql << "\n";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "[DB] SQL ERROR: " << (errMsg ? errMsg : "unknown") << std::endl;
        sqlite3_free(errMsg);
        return false;
    }

    std::cout << "[DB] SQL executed successfully.\n";
    return true;
}


bool EduConnectSystem::initSchema() {
    std::cout << "[DB] initSchema() called; delegating to openDB.\n";
    return openDB("educonnect.db");
}

// add db stuff in
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


    // added in 




    // added in 
    // bool EduConnectSystem::register_student(std::string name,std::string email,std::string password){
    //     std::cout << "[register_student] called with email=" << email << "\n";
    //     if(Students.count(email)){
    //      return false;
    //     }
    //     Student* new_Student = nullptr;
    //     new_Student = new Student(email, name, password);
    //     Students[email] = new_Student;
    //     return true;
    // }

    bool EduConnectSystem::register_student(std::string name,
                                            std::string email,
                                            std::string password)
    {
        std::cout << "[register_student] called with email=" << email << "\n";

        // 🔹 ORIGINAL LOGIC – kept exactly the same
        if (Students.count(email)) {
            std::cout << "[register_student] Student already exists in Students map. "
                        "Skipping creation and DB insert.\n";
            return false;
        }

        Student* new_Student = nullptr;
        new_Student = new Student(name, email, password); 
        // ^ if your ctor is (email, name, password) swap this back, but don't change logic style

        Students[email] = new_Student;
        std::cout << "[register_student] In-memory Student object created and stored in map.\n";

        // 🔹 NEW: Try to also insert into the database (but do NOT change return value)

        if (!db && !openDB("educonnect.db")) {
            std::cerr << "[register_student][DB] WARNING: Could not open DB. "
                        "Student exists in memory only.\n";
            return true;  
        }

        const char* sql =
            "INSERT INTO students (name, email, password) VALUES (?, ?, ?);";

        sqlite3_stmt* stmt = nullptr;
        std::cout << "[register_student][DB] Preparing INSERT for " << email << "\n";

        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            std::cerr << "[register_student][DB] ERROR preparing INSERT: "
                    << sqlite3_errmsg(db) << "\n";
            // don’t undo in-memory success
            return true;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(),  -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_DONE) {
            std::cout << "[register_student][DB] INSERT successful for " << email << "\n";
        } else {
            std::cerr << "[register_student][DB] ERROR during INSERT step: "
                    << sqlite3_errmsg(db) << "\n";
        }

        sqlite3_finalize(stmt);

        std::cout << "[register_student] Completed for " << email << "\n";
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
                                     std::string subject,const std::string description, const std::vector<bool>& days){

        Request* new_request = new Request(s,subject,urgency,description,days);
        for(Tutor* target : selected_tutors){
            target->receive_request(new_request);
        }
        s->add_request(new_request);
}


