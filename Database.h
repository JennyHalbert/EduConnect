#pragma once

#include <string>
#include <sqlite3.h>

class Database {
public:
    explicit Database(const std::string& filename);
    ~Database();

    bool open();
    void close();
    bool initSchema();

    // basic hooks for EduConnectSystem
    bool addStudent(const std::string& name,
                    const std::string& email,
                    const std::string& password);

    bool checkStudentLogin(const std::string& email,
                           const std::string& password);

    bool addTutor(const std::string& name,
                  const std::string& email,
                  const std::string& password);

    bool checkTutorLogin(const std::string& email,
                         const std::string& password);

private:
    std::string filename_;
    sqlite3* db_ = nullptr;
};
