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

    bool addRequest(const std::string& student_email,
        const std::string& tutor_email,
        const std::string& subject,
        int status,
        int urgency,
        const std::string& description,
        const std::vector<bool>& days,
        int is_accepted);
    bool loadAllStudents(std::unordered_map<std::string, Student*>& students);
    bool loadAllTutors(std::unordered_map<std::string, Tutor*>& tutors,
                       std::unordered_map<std::string, std::vector<Tutor*>>& tutors_by_subject);
    bool loadAllRequests(std::unordered_map<std::string, Student*>& students,
                         std::unordered_map<std::string, Tutor*>& tutors);

    

    bool saveAllRequests(const std::unordered_map<std::string, Tutor*>& tutors);

    bool saveAllStudents(const std::unordered_map<std::string, Student*>& students);
    bool saveAllTutors(const std::unordered_map<std::string, Tutor*>& tutors);


    bool updateRequestStatus(const std::string& student_email,
                             const std::string& tutor_email,
                             int status,
                             int is_accepted);
private:
    std::string filename_;
    sqlite3* db_ = nullptr;

    bool ensureTutorColumns();
};

