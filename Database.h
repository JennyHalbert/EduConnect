#pragma once

#include <string>
#include <unordered_map>
#include <sqlite3.h>
#include <vector>

class Student;
class Tutor;

class Database {
public:
    explicit Database(const std::string& filename);
    ~Database();

    bool open();
    void close();
    bool initSchema();

    bool addStudent(const std::string& name,
                    const std::string& email,
                    const std::string& password);

    bool addTutor(const std::string& name,
                  const std::string& email,
                  const std::string& password);

    bool loadAllStudents(std::unordered_map<std::string, Student*>& students);
    bool loadAllTutors(std::unordered_map<std::string, Tutor*>& tutors,
                       std::unordered_map<std::string, std::vector<Tutor*>>& tutors_by_subject);

    // load in request 
    

    bool saveAllStudents(const std::unordered_map<std::string, Student*>& students);
    // bool saveAllTutors(const std::unordered_map<std::string, Tutor*>& tutors);
    bool saveAllTutors(const std::unordered_map<std::string, Tutor*>& tutors);


private:
    std::string filename_;
    sqlite3* db_ = nullptr;

    bool ensureTutorColumns();
};


// // Forward declarations to avoid circular includes
// class Student;
// class Tutor;

// class Database {
// public:
//     explicit Database(const std::string& filename);
//     ~Database();

//     // Open / close underlying SQLite DB
//     bool open();
//     void close();

//     // Create tables if they don't exist yet
//     bool initSchema();

//     // OPTIONAL: still useful if you want to immediately insert on registration.
//     // If you're doing "save on shutdown only", you can keep or remove these.
//     bool addStudent(const std::string& name,
//                     const std::string& email,
//                     const std::string& password);

//     bool addTutor(const std::string& name,
//                   const std::string& email,
//                   const std::string& password);

//     // // NEW: bulk load from DB into your in-memory maps
//     // bool loadAllStudents(std::unordered_map<std::string, Student*>& students);
//     // bool loadAllTutors(std::unordered_map<std::string, Tutor*>& tutors);

//     bool loadAllStudents(std::unordered_map<std::string, Student*>& students);
//     bool loadAllTutors(std::unordered_map<std::string, Tutor*>& tutors,
//                        std::unordered_map<std::string, std::vector<Tutor*>>& tutors_by_subject);
    

//     // NEW: bulk save from your in-memory maps back into DB
//     bool saveAllStudents(const std::unordered_map<std::string, Student*>& students);
//     bool saveAllTutors(const std::unordered_map<std::string, Tutor*>& tutors);



// private:
//     std::string filename_;
//     sqlite3* db_ = nullptr;
// };



// #pragma once

// #include <string>
// #include <sqlite3.h>

// class Database {
// public:
//     explicit Database(const std::string& filename);
//     ~Database();

//     bool open();
//     void close();
//     bool initSchema();

//     // basic hooks for EduConnectSystem
//     bool addStudent(const std::string& name,
//                     const std::string& email,
//                     const std::string& password);

//     bool checkStudentLogin(const std::string& email,
//                            const std::string& password);

//     bool addTutor(const std::string& name,
//                   const std::string& email,
//                   const std::string& password);

//     bool checkTutorLogin(const std::string& email,
//                          const std::string& password);

// private:
//     std::string filename_;
//     sqlite3* db_ = nullptr;
// };
