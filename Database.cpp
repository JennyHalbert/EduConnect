#include "Database.h"
#include <iostream>

Database::Database(const std::string& filename)
    : filename_(filename) {}

Database::~Database() {
    close();
}

bool Database::open() {
    if (db_) return true;
    int rc = sqlite3_open(filename_.c_str(), &db_);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open DB: " << sqlite3_errmsg(db_) << "\n";
        db_ = nullptr;
        return false;
    }
    return true;
}

void Database::close() {
    if (db_) {
        sqlite3_close(db_);
        db_ = nullptr;
    }
}

bool Database::initSchema() {
    if (!open()) return false;

    const char* sql = R"SQL(
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
            -- you can add subjects/days later
        );
    )SQL";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "initSchema error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Database::addStudent(const std::string& name,
                          const std::string& email,
                          const std::string& password) {
    if (!open()) return false;

    const char* sql = "INSERT INTO students (name, email, password) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "addStudent prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "addStudent step error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }
    return true;
}

bool Database::checkStudentLogin(const std::string& email,
                                 const std::string& password) {
    if (!open()) return false;

    const char* sql = "SELECT 1 FROM students WHERE email = ? AND password = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "checkStudentLogin prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    bool ok = (rc == SQLITE_ROW); // got a match

    sqlite3_finalize(stmt);
    return ok;
}

bool Database::addTutor(const std::string& name,
                        const std::string& email,
                        const std::string& password) {
    if (!open()) return false;

    const char* sql = "INSERT INTO tutors (name, email, password) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "addTutor prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "addTutor step error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }
    return true;
}

bool Database::checkTutorLogin(const std::string& email,
                               const std::string& password) {
    if (!open()) return false;

    const char* sql = "SELECT 1 FROM tutors WHERE email = ? AND password = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "checkTutorLogin prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    bool ok = (rc == SQLITE_ROW);

    sqlite3_finalize(stmt);
    return ok;
}
