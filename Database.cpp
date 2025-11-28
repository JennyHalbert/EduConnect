#include "Database.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "Student.h"
#include "Tutor.h"
#include "Request.h"

Database::Database(const std::string& filename)
    : filename_(filename) {}

Database::~Database() {
    close();
}


// --- small helpers ----------------------------------------------------

// "1010100" -> vector<bool>
static std::vector<bool> decodeDays(const std::string& s) {
    std::vector<bool> days;
    days.reserve(s.size());
    for (char c : s) {
        days.push_back(c == '1');
    }
    return days;
}

// vector<bool> -> "1010100"
static std::string encodeDays(const std::vector<bool>& days) {
    std::string s;
    s.reserve(days.size());
    for (bool d : days) {
        s.push_back(d ? '1' : '0');
    }
    return s;
}

// small wrapper around sqlite3_exec so saveAllTutors can call execSQL(...)
static bool execSQL(sqlite3* db, const char* sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "execSQL error: " << (errMsg ? errMsg : "unknown") << "\n";
        if (errMsg) sqlite3_free(errMsg);
        return false;
    }
    return true;
}


bool Database::open() {
    if (db_) return true;  // already open

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
            id             INTEGER PRIMARY KEY AUTOINCREMENT,
            name           TEXT NOT NULL,
            email          TEXT NOT NULL UNIQUE,
            password       TEXT NOT NULL,
            days           TEXT NOT NULL,    -- e.g. "1010100" for Mon, Wed, Fri
            total_ratings  REAL NOT NULL,
            total_completed INTEGER NOT NULL,
            total_matched  INTEGER NOT NULL
        );

        CREATE TABLE IF NOT EXISTS subjects (
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            tutor_email TEXT NOT NULL,
            subject     TEXT NOT NULL,
            FOREIGN KEY(tutor_email) REFERENCES tutors(email)
        );

        CREATE TABLE IF NOT EXISTS requests (
            id            INTEGER PRIMARY KEY AUTOINCREMENT,
            student_email TEXT NOT NULL,
            tutor_email   TEXT NOT NULL,
            subject       TEXT NOT NULL,
            description   TEXT,
            urgency       INTEGER NOT NULL,
            status        INTEGER NOT NULL,
            is_accepted   INTEGER NOT NULL,  -- 1 = active, 0 = previous
            days          TEXT NOT NULL,     -- e.g. "1010100" for Mon, Wed, Fri
            FOREIGN KEY(student_email) REFERENCES students(email),
            FOREIGN KEY(tutor_email)   REFERENCES tutors(email)
        );
    )SQL";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "initSchema error: " << errMsg << "\n";
        sqlite3_free(errMsg);
        return false;
    }

    // Migrate old DBs that had only 4 tutor columns.
    if (!ensureTutorColumns()) {
        std::cerr << "initSchema error: failed to ensure tutor columns.\n";
        return false;
    }

    return true;
}

// ===== optional single-row helpers ===================================

bool Database::addStudent(const std::string& name,
                          const std::string& email,
                          const std::string& password) {
    if (!open()) return false;

    const char* sql =
        "INSERT INTO students (name, email, password) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "addStudent prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "addStudent step error: " << sqlite3_errmsg(db_) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// ⚠️ Note: this addTutor no longer matches the tutors schema (it ignores days/stats).
// For now you are not using it; if you do, we must update it to insert all columns.
bool Database::addTutor(const std::string& name,
                        const std::string& email,
                        const std::string& password) {
    if (!open()) return false;

    const char* sql =
        "INSERT INTO tutors (name, email, password, days, total_ratings, total_completed, total_matched) "
        "VALUES (?, ?, ?, '0000000', 0.0, 0, 0);";

    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "addTutor prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(),  -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "addTutor step error: " << sqlite3_errmsg(db_) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// bool Database::addRequest(const std::string& student_email,
//                           const std::string& tutor_email,
//                           const std::string& subject,
//                           int status,
//                           int urgency,
//                           const std::string& description,
//                           const std::vector<bool>& days,
//                           int is_accepted) {
//     if (!open()) return false;

//     const char* sql =
//         "INSERT INTO requests "
//         "(student_email, tutor_email, subject, description, urgency, status, is_accepted, days) "
//         "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

//     sqlite3_stmt* stmt = nullptr;
//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "addRequest prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     std::string daysStr = encodeDays(days);

//     sqlite3_bind_text(stmt, 1, student_email.c_str(), -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 2, tutor_email.c_str(),   -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 3, subject.c_str(),       -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 4, description.c_str(),   -1, SQLITE_TRANSIENT);
//     sqlite3_bind_int (stmt, 5, urgency);
//     sqlite3_bind_int (stmt, 6, status);
//     sqlite3_bind_int (stmt, 7, is_accepted);
//     sqlite3_bind_text(stmt, 8, daysStr.c_str(),       -1, SQLITE_TRANSIENT);

//     int rc = sqlite3_step(stmt);
//     if (rc != SQLITE_DONE) {
//         std::cerr << "addRequest step error: " << sqlite3_errmsg(db_) << "\n";
//         sqlite3_finalize(stmt);
//         return false;
//     }

//     sqlite3_finalize(stmt);
//     return true;
// }

// bool Database::updateRequestStatus(const std::string& student_email,
//                                    const std::string& tutor_email,
//                                    int status,
//                                    int is_accepted) {
//     if (!open()) return false;

//     const char* sql =
//         "UPDATE requests "
//         "SET status = ?, is_accepted = ? "
//         "WHERE id = ("
//         "  SELECT id FROM requests "
//         "  WHERE student_email = ? AND tutor_email = ? "
//         "  ORDER BY id DESC LIMIT 1"
//         ");";

//     sqlite3_stmt* stmt = nullptr;
//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "updateRequestStatus prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     sqlite3_bind_int (stmt, 1, status);
//     sqlite3_bind_int (stmt, 2, is_accepted);
//     sqlite3_bind_text(stmt, 3, student_email.c_str(), -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 4, tutor_email.c_str(),   -1, SQLITE_TRANSIENT);

//     int rc = sqlite3_step(stmt);
//     if (rc != SQLITE_DONE) {
//         std::cerr << "updateRequestStatus step error: " << sqlite3_errmsg(db_) << "\n";
//         sqlite3_finalize(stmt);
//         return false;
//     }

//     sqlite3_finalize(stmt);
//     return sqlite3_changes(db_) > 0;
// }

// // ===== bulk LOAD: DB -> in-memory maps ===============================

// bool Database::saveAllRequests(const std::vector<Request*>& requests) {
//     if (!open()) return false;

//     const char* sql = "INSERT INTO requests (student_email, tutor_email, subject, description, urgency, status, is_accepted, days) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
//     sqlite3_stmt* stmt = nullptr;

//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "saveAllRequests prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     for (const Request* req : requests) {
//         if (!req) continue;

//         sqlite3_reset(stmt);
//         sqlite3_clear_bindings(stmt);

//         sqlite3_bind_text(stmt, 1, req->get_student_email().c_str(), -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmt, 2, req->get_tutor_email().c_str(), -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmt, 3, req->get_subject().c_str(), -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmt, 4, req->get_description().c_str(), -1, SQLITE_TRANSIENT);
//         sqlite3_bind_int(stmt, 5, req->get_urgency());
//         sqlite3_bind_int(stmt, 6, req->get_status());
//         sqlite3_bind_int(stmt, 7, req->is_accepted() ? 1 : 0);
//         sqlite3_bind_text(stmt, 8, encodeDays(req->get_days()).c_str(), -1, SQLITE_TRANSIENT);

//         if (sqlite3_step(stmt) != SQLITE_DONE) {
//             std::cerr << "saveAllRequests step error: " << sqlite3_errmsg(db_) << "\n";
//         }
//     }

//     sqlite3_finalize(stmt);
//     return true;
// }


bool Database::loadAllStudents(std::unordered_map<std::string, Student*>& students) {
    if (!open()) return false;

    const char* sql = "SELECT name, email, password FROM students;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "loadAllStudents prepare error: "
                  << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    int count = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* name_c  =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* email_c =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* pass_c  =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        std::string name  = name_c  ? name_c  : "";
        std::string email = email_c ? email_c : "";
        std::string pass  = pass_c  ? pass_c  : "";

        if (email.empty()) {
            continue;
        }

        if (students.count(email)) {
            // already loaded
            continue;
        }

        // Student(email, name, password)
        Student* s = new Student(email, name, pass);
        students[email] = s;
        ++count;
    }

    sqlite3_finalize(stmt);

    std::cout << "loadAllStudents: loaded " << count << " students from DB.\n";
    return true;
}


// bool Database::loadAllStudents(std::unordered_map<std::string, Student*>& students) {
//     if (!open()) return false;

//     const char* sql = "SELECT name, email, password FROM students;";
//     sqlite3_stmt* stmt = nullptr;

//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "loadAllStudents prepare error: "
//                   << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     int count = 0;

//     while (sqlite3_step(stmt) == SQLITE_ROW) {
//         const char* name_c  =
//             reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
//         const char* email_c =
//             reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
//         const char* pass_c  =
//             reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

//         std::string name  = name_c  ? name_c  : "";
//         std::string email = email_c ? email_c : "";
//         std::string pass  = pass_c  ? pass_c  : "";

//         if (email.empty()) {
//             continue;
//         }

//         if (students.count(email)) {
//             // already loaded
//             continue;
//         }

//         // Student(email, name, password)
//         Student* s = new Student(email, name, pass);
//         students[email] = s;
//         ++count;
//     }

//     sqlite3_finalize(stmt);

//     std::cout << "loadAllStudents: loaded " << count << " students from DB.\n";
//     return true;
// }

bool Database::saveAllRequests(std::vector<Request*>& requests) {
    if (!open()) return false;

    std::cout << "[DB] saveAllRequests: saving " << requests.size() << " requests.\n";

    // 1) Clear existing rows (bulk overwrite strategy)
    if (!execSQL(db_, "DELETE FROM requests;")) {
        std::cerr << "saveAllRequests: failed to clear requests table.\n";
        return false;
    }

    // 2) Prepare INSERT matching your current schema
    // requests table: student_email, tutor_email, subject, description,
    //                 urgency, status, is_accepted, days
    const char* sql =
        "INSERT INTO requests "
        "(student_email, tutor_email, subject, description, "
        " urgency, status, is_accepted, days) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "saveAllRequests prepare error: "
                  << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    int count = 0;

    for (Request* req : requests) {
        if (!req) continue;

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);

        // 1: student_email
        std::string student_email;
        if (Student* s = req->get_student()) {
            student_email = s->get_email();
        }
        sqlite3_bind_text(stmt, 1, student_email.c_str(), -1, SQLITE_TRANSIENT);

        // 2: tutor_email (empty if no tutor yet)
        std::string tutor_email;
        if (Tutor* t = req->get_tutor()) {
            tutor_email = t->get_email();
        }
        sqlite3_bind_text(stmt, 2, tutor_email.c_str(), -1, SQLITE_TRANSIENT);

        // 3: subject
        sqlite3_bind_text(stmt, 3, req->get_subject().c_str(), -1, SQLITE_TRANSIENT);

        // 4: description
        sqlite3_bind_text(stmt, 4, req->get_description().c_str(), -1, SQLITE_TRANSIENT);

        // 5: urgency (enum -> int)
        sqlite3_bind_int(stmt, 5, static_cast<int>(req->get_urgency()));

        // 6: status (enum -> int)
        sqlite3_bind_int(stmt, 6, static_cast<int>(req->get_status()));

        // 7: is_accepted (bool -> int)
        sqlite3_bind_int(stmt, 7, req->get_is_accepted() ? 1 : 0);

        // 8: days (vector<bool> -> "1010100")
        std::string daysStr = encodeDays(req->get_days());
        sqlite3_bind_text(stmt, 8, daysStr.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            std::cerr << "saveAllRequests step error: "
                      << sqlite3_errmsg(db_) << "\n";
        } else {
            ++count;
        }
    }

    sqlite3_finalize(stmt);

    std::cout << "[DB] saveAllRequests: saved " << count << " requests.\n";
    return true;
}


bool Database::addRequest(const std::string& student_email,
    const std::string& tutor_email,
    const std::string& subject,
    int status,
    int urgency,
    const std::string& description,
    const std::vector<bool>& days,
    bool is_accepted) { // Change int to bool
if (!open()) return false;

const char* sql =
"INSERT INTO requests "
"(student_email, tutor_email, subject, description, urgency, status, is_accepted, days) "
"VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

sqlite3_stmt* stmt = nullptr;
if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
std::cerr << "addRequest prepare error: " << sqlite3_errmsg(db_) << "\n";
return false;
}

std::string daysStr = encodeDays(days);

sqlite3_bind_text(stmt, 1, student_email.c_str(), -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 2, tutor_email.c_str(),   -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 3, subject.c_str(),       -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 4, description.c_str(),   -1, SQLITE_TRANSIENT);
sqlite3_bind_int (stmt, 5, urgency);
sqlite3_bind_int (stmt, 6, status);
sqlite3_bind_int (stmt, 7, is_accepted ? 1 : 0); // Convert bool to int
sqlite3_bind_text(stmt, 8, daysStr.c_str(),       -1, SQLITE_TRANSIENT);

int rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
std::cerr << "addRequest step error: " << sqlite3_errmsg(db_) << "\n";
sqlite3_finalize(stmt);
return false;
}

sqlite3_finalize(stmt);
return true;
}

bool Database::updateRequestStatus(const std::string& student_email,
             const std::string& tutor_email,
             int status,
             bool is_accepted) { // Change int to bool
if (!open()) return false;

const char* sql =
"UPDATE requests "
"SET status = ?, is_accepted = ? "
"WHERE id = ("
"  SELECT id FROM requests "
"  WHERE student_email = ? AND tutor_email = ? "
"  ORDER BY id DESC LIMIT 1"
");";

sqlite3_stmt* stmt = nullptr;
if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
std::cerr << "updateRequestStatus prepare error: " << sqlite3_errmsg(db_) << "\n";
return false;
}

sqlite3_bind_int (stmt, 1, status);
sqlite3_bind_int (stmt, 2, is_accepted ? 1 : 0); // Convert bool to int
sqlite3_bind_text(stmt, 3, student_email.c_str(), -1, SQLITE_TRANSIENT);
sqlite3_bind_text(stmt, 4, tutor_email.c_str(),   -1, SQLITE_TRANSIENT);

int rc = sqlite3_step(stmt);
if (rc != SQLITE_DONE) {
std::cerr << "updateRequestStatus step error: " << sqlite3_errmsg(db_) << "\n";
sqlite3_finalize(stmt);
return false;
}

sqlite3_finalize(stmt);
return sqlite3_changes(db_) > 0;
}


bool Database::loadAllTutors(std::unordered_map<std::string, Tutor*>& tutors,
    std::unordered_map<std::string, std::vector<Tutor*>>& tutors_by_subject)
{
    // 1) Load tutor rows
    if (!open()) return false;

    const char* sqlTut =
        "SELECT name, email, password, days, total_ratings, total_completed, total_matched "
        "FROM tutors;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, sqlTut, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "loadAllTutors prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    int count = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* name_c     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* email_c    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        const char* password_c = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        const char* days_c     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        double total_ratings   = sqlite3_column_double(stmt, 4);
        int total_completed    = sqlite3_column_int(stmt, 5);
        int total_matched      = sqlite3_column_int(stmt, 6);

        std::string name     = name_c     ? name_c     : "";
        std::string email    = email_c    ? email_c    : "";
        std::string password = password_c ? password_c : "";
        std::string days_str = days_c     ? days_c     : "";

        if (email.empty()) continue;

        std::vector<bool> days_vec = decodeDays(days_str);
        if (days_vec.empty()) days_vec.assign(7, false);

        std::vector<std::string> empty_subjects;

        Tutor* t = new Tutor(email, name, password, days_vec, empty_subjects);

        // TODO: if you have a set_totals(...) method, call it here:
        // t->set_totals(total_ratings, total_completed, total_matched);

        tutors[email] = t;
        ++count;
    }

    sqlite3_finalize(stmt);

    std::cout << "loadAllTutors: loaded " << count << " tutors from DB (no subjects yet).\n";

    // 2) Load subjects and attach + index
    const char* sqlSubj =
        "SELECT tutor_email, subject FROM subjects;";

    if (sqlite3_prepare_v2(db_, sqlSubj, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "loadAllTutors subjects prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    int subjCount = 0;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char* email_c = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        const char* subj_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        std::string email   = email_c ? email_c : "";
        std::string subject = subj_c  ? subj_c  : "";

        if (email.empty() || subject.empty()) continue;

        auto it = tutors.find(email);
        if (it == tutors.end()) continue;

        Tutor* t = it->second;

        auto current_subs = t->get_subjects();
        current_subs.push_back(subject);
        t->set_subjects(current_subs);

        tutors_by_subject[subject].push_back(t);

        ++subjCount;
    }

    sqlite3_finalize(stmt);

    std::cout << "loadAllTutors: attached " << subjCount << " subject rows to tutors.\n";
    return true;
}


// ===== bulk SAVE: in-memory maps -> DB ===============================

bool Database::saveAllStudents(const std::unordered_map<std::string, Student*>& students) {
    if (!open()) return false;

    // Clear existing rows for a clean overwrite (fine for a school project)
    {
        const char* sqlDel = "DELETE FROM students;";
        char* errMsg = nullptr;
        int rc = sqlite3_exec(db_, sqlDel, nullptr, nullptr, &errMsg);
        if (rc != SQLITE_OK) {
            std::cerr << "saveAllStudents DELETE error: " << errMsg << "\n";
            sqlite3_free(errMsg);
            // keep going anyway
        }
    }

    const char* sql =
        "INSERT INTO students (name, email, password) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "saveAllStudents prepare error: "
                  << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    int count = 0;

    for (const auto& pair : students) {
        const std::string& email = pair.first;
        Student* s = pair.second;
        if (!s) continue;

        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);

        sqlite3_bind_text(stmt, 1, s->get_name().c_str(),     -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, s->get_email().c_str(),    -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, s->get_password().c_str(), -1, SQLITE_TRANSIENT);

        int rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "saveAllStudents INSERT error for " << email
                      << ": " << sqlite3_errmsg(db_) << "\n";
        } else {
            ++count;
        }
    }

    sqlite3_finalize(stmt);

    std::cout << "saveAllStudents: saved " << count << " students to DB.\n";
    return true;
}


bool Database::saveAllTutors(const std::unordered_map<std::string, Tutor*>& tutors) {
    if (!open()) return false;

    std::cout << "[DB] saveAllTutors: saving " << tutors.size() << " tutors\n";

    // Clear subjects first, then tutors (we'll repopulate them)
    if (!execSQL(db_, "DELETE FROM subjects;")) {
        std::cerr << "saveAllTutors: failed to clear subjects table.\n";
        return false;
    }
    if (!execSQL(db_, "DELETE FROM tutors;")) {
        std::cerr << "saveAllTutors: failed to clear tutors table.\n";
        return false;
    }

    // ---------- insert into tutors ----------
    const char* sqlTut =
        "INSERT INTO tutors "
        "(name, email, password, days, total_ratings, total_completed, total_matched) "
        "VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmtTut = nullptr;

    if (sqlite3_prepare_v2(db_, sqlTut, -1, &stmtTut, nullptr) != SQLITE_OK) {
        std::cerr << "saveAllTutors prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    for (const auto& pair : tutors) {
        Tutor* t = pair.second;  // ✅ NOT const
        if (!t) continue;

        sqlite3_reset(stmtTut);
        sqlite3_clear_bindings(stmtTut);

        // encodeDays uses Tutor::get_days(), which is non-const — this is fine now
        std::string daysStr = encodeDays(t->get_days());

        sqlite3_bind_text(stmtTut, 1, t->get_name().c_str(),     -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtTut, 2, t->get_email().c_str(),    -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtTut, 3, t->get_password().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtTut, 4, daysStr.c_str(),           -1, SQLITE_TRANSIENT);

        double avg        = t->avg_rating();
        int    completed  = t->get_completed();
        double total_ratings = avg * completed;

        sqlite3_bind_double(stmtTut, 5, total_ratings);
        sqlite3_bind_int   (stmtTut, 6, completed);
        sqlite3_bind_int   (stmtTut, 7, t->get_matched());

        int rc = sqlite3_step(stmtTut);
        if (rc != SQLITE_DONE) {
            std::cerr << "saveAllTutors tutor step error for email "
                      << t->get_email() << ": "
                      << sqlite3_errmsg(db_) << "\n";
        }
    }

    sqlite3_finalize(stmtTut);

    // ---------- insert into subjects ----------
    const char* sqlSubj =
        "INSERT INTO subjects (tutor_email, subject) VALUES (?, ?);";

    sqlite3_stmt* stmtSubj = nullptr;

    if (sqlite3_prepare_v2(db_, sqlSubj, -1, &stmtSubj, nullptr) != SQLITE_OK) {
        std::cerr << "saveAllTutors subjects prepare error: " << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    for (const auto& pair : tutors) {
        Tutor* t = pair.second;  
        if (!t) continue;

        auto subs = t->get_subjects();  // returns by value, local copy is fine

        for (const std::string& subj : subs) {
            sqlite3_reset(stmtSubj);
            sqlite3_clear_bindings(stmtSubj);

            sqlite3_bind_text(stmtSubj, 1, t->get_email().c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmtSubj, 2, subj.c_str(),          -1, SQLITE_TRANSIENT);

            int rc = sqlite3_step(stmtSubj);
            if (rc != SQLITE_DONE) {
                std::cerr << "saveAllTutors subject step error for tutor "
                          << t->get_email() << ": "
                          << sqlite3_errmsg(db_) << "\n";
            }
        }
    }

    sqlite3_finalize(stmtSubj);
    std::cout << "[DB] saveAllTutors: finished.\n";
    return true;
}

// bool Database::loadAllRequests(std::unordered_map<std::string, Student*>& students,
//                                std::unordered_map<std::string, Tutor*>& tutors) {
//     if (!open()) return false;

//     const char* sql =
//         "SELECT student_email, tutor_email, subject, description, urgency, status, is_accepted, days "
//         "FROM requests;";

//     sqlite3_stmt* stmt = nullptr;
//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "loadAllRequests prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     int count = 0;
//     int skipped = 0;

//     while (sqlite3_step(stmt) == SQLITE_ROW) {
//         const char* stu_c   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
//         const char* tut_c   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
//         const char* subj_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
//         const char* desc_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
//         int urgency_i       = sqlite3_column_int(stmt, 4);
//         int status_i        = sqlite3_column_int(stmt, 5);
//         int is_accepted_i   = sqlite3_column_int(stmt, 6);
//         const char* days_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));

//         std::string stuEmail = stu_c  ? stu_c  : "";
//         std::string tutEmail = tut_c  ? tut_c  : "";
//         std::string subject  = subj_c ? subj_c : "";
//         std::string descr    = desc_c ? desc_c : "";
//         std::string daysStr  = days_c ? days_c : "";

//         auto stuIt = students.find(stuEmail);
//         auto tutIt = tutors.find(tutEmail);

//         if (stuIt == students.end() || tutIt == tutors.end()) {
//             ++skipped;
//             continue;
//         }

//         Student* s = stuIt->second;
//         Tutor*   t = tutIt->second;

//         std::vector<bool> daysVec = decodeDays(daysStr);
//         if (daysVec.empty()) daysVec.assign(7, false);

//         Request::UrgencyLevel urg =
//             static_cast<Request::UrgencyLevel>(urgency_i < 0 ? 0 : urgency_i);
//         Request::RequestStatus stat =
//             static_cast<Request::RequestStatus>(status_i < 0 ? 0 : status_i);

//         Request* r = new Request(t, s, subject, stat, urg, descr, daysVec);
//         r->update_is_accepted(is_accepted_i != 0);

//         s->add_request(r);

//         if (stat == Request::POSTED) {
//             t->receive_request(r);
//         } else if (stat == Request::MATCHED || stat == Request::COMPLETED) {
//             r->update_status(Request::POSTED);
//             t->accept_request(r);  // moves into active + sets tutor pointer
//             r->update_status(stat);
//         }

//         ++count;
//     }

//     sqlite3_finalize(stmt);

//     std::cout << "[DB] loadAllRequests: loaded " << count
//               << " requests from DB"
//               << (skipped ? (", skipped " + std::to_string(skipped) + " with missing users.") : ".")
//               << "\n";
//     return true;
// }

bool Database::loadAllRequests(std::unordered_map<std::string, Student*>& students,
    std::unordered_map<std::string, Tutor*>& tutors,
    std::vector<Request*>& allRequests) { 
if (!open()) return false;

// Select all columns
const char* sql =
"SELECT student_email, tutor_email, subject, description, urgency, status, is_accepted, days "
"FROM requests;";

sqlite3_stmt* stmt = nullptr;
if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
std::cerr << "loadAllRequests prepare error: " << sqlite3_errmsg(db_) << "\n";
return false;
}

int count = 0;
int skipped = 0;

while (sqlite3_step(stmt) == SQLITE_ROW) {
const char* stu_c   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
const char* tut_c   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)); 
const char* subj_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
const char* desc_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
int urgency_i       = sqlite3_column_int(stmt, 4);
int status_i        = sqlite3_column_int(stmt, 5);
int is_accepted_i   = sqlite3_column_int(stmt, 6);
const char* days_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));

std::string stuEmail = stu_c  ? stu_c  : "";
std::string tutEmail = tut_c  ? tut_c  : "";
std::string subject  = subj_c ? subj_c : "";
std::string descr    = desc_c ? desc_c : "";
std::string daysStr  = days_c ? days_c : "";

// 1. Find the Student (Required)
auto stuIt = students.find(stuEmail);
if (stuIt == students.end()) {
++skipped; 
continue;
}
Student* s = stuIt->second;

// 2. Find the Tutor (Optional - allows loading POSTED requests)
Tutor* t = nullptr;
if (!tutEmail.empty()) {
auto tutIt = tutors.find(tutEmail);
if (tutIt != tutors.end()) {
t = tutIt->second;
}
}

std::vector<bool> daysVec = decodeDays(daysStr);
if (daysVec.empty()) daysVec.assign(7, false);

Request::UrgencyLevel urg =
static_cast<Request::UrgencyLevel>(urgency_i < 0 ? 0 : urgency_i);
Request::RequestStatus stat =
static_cast<Request::RequestStatus>(status_i < 0 ? 0 : status_i);

// 3. Create the Request
Request* r = new Request(t, s, subject, stat, urg, descr, daysVec);
r->update_is_accepted(is_accepted_i != 0);

// 4. CRITICAL: Store in main vector so it is saved later!
allRequests.push_back(r);

// Link to Student
s->add_request(r);

// Link to Tutor (if one exists)
if (t) {
// Restore tutor state
if (stat == Request::POSTED) {
t->receive_request(r);
} else {
// Determine if it should be in active or history
// We simulate the flow to put it in the right list:
r->update_status(Request::POSTED);
t->receive_request(r);
t->accept_request(r); 
r->update_status(stat); // Restore actual status

if (stat == Request::COMPLETED) {
t->close_request(r);
}
}
}
++count;
}

sqlite3_finalize(stmt);
std::cout << "[DB] loadAllRequests: loaded " << count << " requests.\n";
return true;
}

// Ensure legacy DBs have the new tutor columns; safe to call repeatedly.
bool Database::ensureTutorColumns() {
    if (!open()) return false;

    const char* pragma = "PRAGMA table_info(tutors);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db_, pragma, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "ensureTutorColumns prepare error: "
                  << sqlite3_errmsg(db_) << "\n";
        return false;
    }

    std::unordered_set<std::string> cols;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 1);
        if (text) {
            cols.insert(reinterpret_cast<const char*>(text));
        }
    }
    sqlite3_finalize(stmt);

    if (!cols.count("days")) {
        if (!execSQL(db_, "ALTER TABLE tutors ADD COLUMN days TEXT NOT NULL DEFAULT '0000000';")) {
            return false;
        }
    }
    if (!cols.count("total_ratings")) {
        if (!execSQL(db_, "ALTER TABLE tutors ADD COLUMN total_ratings REAL NOT NULL DEFAULT 0.0;")) {
            return false;
        }
    }
    if (!cols.count("total_completed")) {
        if (!execSQL(db_, "ALTER TABLE tutors ADD COLUMN total_completed INTEGER NOT NULL DEFAULT 0;")) {
            return false;
        }
    }
    if (!cols.count("total_matched")) {
        if (!execSQL(db_, "ALTER TABLE tutors ADD COLUMN total_matched INTEGER NOT NULL DEFAULT 0;")) {
            return false;
        }
    }

    return true;
}




// #include "Database.h"

// #include <iostream>
// #include <unordered_map>
// #include <vector>

// #include "Student.h"
// #include "Tutor.h"



// Database::Database(const std::string& filename)
//     : filename_(filename) {}

// Database::~Database() {
//     close();
// }


// // --- small helpers ----------------------------------------------------

// // "1010100" -> vector<bool>
// static std::vector<bool> decodeDays(const std::string& s) {
//     std::vector<bool> days;
//     days.reserve(s.size());
//     for (char c : s) {
//         days.push_back(c == '1');
//     }
//     return days;
// }

// // (optional) vector<bool> -> "1010100" if you're also using encodeDays elsewhere
// static std::string encodeDays(const std::vector<bool>& days) {
//     std::string s;
//     s.reserve(days.size());
//     for (bool d : days) {
//         s.push_back(d ? '1' : '0');
//     }
//     return s;
// }



// bool Database::open() {
//     if (db_) return true;  // already open

//     int rc = sqlite3_open(filename_.c_str(), &db_);
//     if (rc != SQLITE_OK) {
//         std::cerr << "Cannot open DB: " << sqlite3_errmsg(db_) << "\n";
//         db_ = nullptr;
//         return false;
//     }
//     return true;
// }

// void Database::close() {
//     if (db_) {
//         sqlite3_close(db_);
//         db_ = nullptr;
//     }
// }

// bool Database::initSchema() {
//     if (!open()) return false;

//     const char* sql = R"SQL(
//         CREATE TABLE IF NOT EXISTS students (
//             id       INTEGER PRIMARY KEY AUTOINCREMENT,
//             name     TEXT NOT NULL,
//             email    TEXT NOT NULL UNIQUE,
//             password TEXT NOT NULL
//         );

//         CREATE TABLE IF NOT EXISTS tutors (
//             id       INTEGER PRIMARY KEY AUTOINCREMENT,
//             name     TEXT NOT NULL,
//             email    TEXT NOT NULL UNIQUE,
//             password TEXT NOT NULL,
//             days     TEXT NOT NULL,    -- e.g. "1010100" for Mon, Wed, Fri
//             total_ratings REAL NOT NULL,
//             total_completed INTEGER NOT NULL,
//             total_matched INTEGER NOT NULL

//         );

//         CREATE TABLE IF NOT EXISTS subjects (
//             id        INTEGER PRIMARY KEY AUTOINCREMENT,
//             tutor_email TEXT NOT NULL,
//             subject   TEXT NOT NULL,
//             FOREIGN KEY(tutor_email) REFERENCES tutors(email)
//         );

//         CREATE TABLE IF NOT EXISTS requests (
//             id            INTEGER PRIMARY KEY AUTOINCREMENT,
//             student_email TEXT NOT NULL,
//             tutor_email   TEXT NOT NULL,
//             subject       TEXT NOT NULL,
//             description   TEXT,
//             urgency       INTEGER NOT NULL,
//             status        INTEGER NOT NULL,
//             is_accepted     INTEGER NOT NULL,  -- 1 = active, 0 = previous
//             days         TEXT NOT NULL,     -- e.g. "1010100" for Mon, Wed, Fri
//             FOREIGN KEY(student_email) REFERENCES students(email),
//             FOREIGN KEY(tutor_email)   REFERENCES tutors(email)
//         );
//     )SQL";


//     char* errMsg = nullptr;
//     int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errMsg);
//     if (rc != SQLITE_OK) {
//         std::cerr << "initSchema error: " << errMsg << "\n";
//         sqlite3_free(errMsg);
//         return false;
//     }
//     return true;
// }

// // ===== optional single-row helpers ===================================

// bool Database::addStudent(const std::string& name,
//                           const std::string& email,
//                           const std::string& password) {
//     if (!open()) return false;

//     const char* sql =
//         "INSERT INTO students (name, email, password) VALUES (?, ?, ?);";

//     sqlite3_stmt* stmt = nullptr;

//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "addStudent prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     sqlite3_bind_text(stmt, 1, name.c_str(),  -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

//     int rc = sqlite3_step(stmt);
//     if (rc != SQLITE_DONE) {
//         std::cerr << "addStudent step error: " << sqlite3_errmsg(db_) << "\n";
//         sqlite3_finalize(stmt);
//         return false;
//     }

//     sqlite3_finalize(stmt);
//     return true;
// }

// bool Database::addTutor(const std::string& name,
//                         const std::string& email,
//                         const std::string& password) {
//     if (!open()) return false;

//     const char* sql =
//         "INSERT INTO tutors (name, email, password) VALUES (?, ?, ?);";

//     sqlite3_stmt* stmt = nullptr;

//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "addTutor prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     sqlite3_bind_text(stmt, 1, name.c_str(),  -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
//     sqlite3_bind_text(stmt, 3, password.c_str(), -1, SQLITE_TRANSIENT);

//     int rc = sqlite3_step(stmt);
//     if (rc != SQLITE_DONE) {
//         std::cerr << "addTutor step error: " << sqlite3_errmsg(db_) << "\n";
//         sqlite3_finalize(stmt);
//         return false;
//     }

//     sqlite3_finalize(stmt);
//     return true;
// }

// // ===== bulk LOAD: DB -> in-memory maps ===============================

// bool Database::loadAllStudents(std::unordered_map<std::string, Student*>& students) {
//     if (!open()) return false;

//     const char* sql = "SELECT name, email, password FROM students;";
//     sqlite3_stmt* stmt = nullptr;

//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "loadAllStudents prepare error: "
//                   << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     int count = 0;

//     while (sqlite3_step(stmt) == SQLITE_ROW) {
//         const char* name_c  =
//             reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
//         const char* email_c =
//             reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
//         const char* pass_c  =
//             reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

//         std::string name  = name_c  ? name_c  : "";
//         std::string email = email_c ? email_c : "";
//         std::string pass  = pass_c  ? pass_c  : "";

//         if (email.empty()) {
//             continue;
//         }

//         if (students.count(email)) {
//             // already loaded
//             continue;
//         }

//         // Student(email, name, password)
//         Student* s = new Student(email, name, pass);
//         students[email] = s;
//         ++count;
//     }

//     sqlite3_finalize(stmt);

//     std::cout << "loadAllStudents: loaded " << count << " students from DB.\n";
//     return true;
// }



// bool Database::loadAllTutors(std::unordered_map<std::string, Tutor*>& tutors,
//     std::unordered_map<std::string, std::vector<Tutor*>>& tutors_by_subject)
// {
// // 1) Load tutor rows
// if (!open()) return false;

// const char* sqlTut =
// "SELECT name, email, password, days, total_ratings, total_completed, total_matched "
// "FROM tutors;";

// sqlite3_stmt* stmt = nullptr;
// if (sqlite3_prepare_v2(db_, sqlTut, -1, &stmt, nullptr) != SQLITE_OK) {
// std::cerr << "loadAllTutors prepare error: " << sqlite3_errmsg(db_) << "\n";
// return false;
// }

// int count = 0;

// while (sqlite3_step(stmt) == SQLITE_ROW) {
// const char* name_c     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
// const char* email_c    = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
// const char* password_c = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
// const char* days_c     = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
// double total_ratings   = sqlite3_column_double(stmt, 4);
// int total_completed    = sqlite3_column_int(stmt, 5);
// int total_matched      = sqlite3_column_int(stmt, 6);

// std::string name     = name_c     ? name_c     : "";
// std::string email    = email_c    ? email_c    : "";
// std::string password = password_c ? password_c : "";
// std::string days_str = days_c     ? days_c     : "";

// if (email.empty()) continue;

// std::vector<bool> days_vec = decodeDays(days_str);
// if (days_vec.empty()) days_vec.assign(7, false);

// std::vector<std::string> empty_subjects;

// Tutor* t = new Tutor(email, name, password, days_vec, empty_subjects);

// // TODO: if you have a set_totals(...) method, call it here:
// // t->set_totals(total_ratings, total_completed, total_matched);

// tutors[email] = t;
// ++count;
// }

// sqlite3_finalize(stmt);

// std::cout << "loadAllTutors: loaded " << count << " tutors from DB (no subjects yet).\n";

// // 2) Load subjects and attach + index
// const char* sqlSubj =
// "SELECT tutor_email, subject FROM subjects;";

// if (sqlite3_prepare_v2(db_, sqlSubj, -1, &stmt, nullptr) != SQLITE_OK) {
// std::cerr << "loadAllTutors subjects prepare error: " << sqlite3_errmsg(db_) << "\n";
// return false;
// }

// int subjCount = 0;

// while (sqlite3_step(stmt) == SQLITE_ROW) {
// const char* email_c = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
// const char* subj_c  = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

// std::string email   = email_c ? email_c : "";
// std::string subject = subj_c  ? subj_c  : "";

// if (email.empty() || subject.empty()) continue;

// auto it = tutors.find(email);
// if (it == tutors.end()) continue;

// Tutor* t = it->second;

// auto current_subs = t->get_subjects();
// current_subs.push_back(subject);
// t->set_subjects(current_subs);

// tutors_by_subject[subject].push_back(t);

// ++subjCount;
// }

// sqlite3_finalize(stmt);

// std::cout << "loadAllTutors: attached " << subjCount << " subject rows to tutors.\n";
// return true;
// }


// // ===== bulk SAVE: in-memory maps -> DB ===============================

// bool Database::saveAllStudents(const std::unordered_map<std::string, Student*>& students) {
//     if (!open()) return false;

//     // Clear existing rows for a clean overwrite (fine for a school project)
//     {
//         const char* sqlDel = "DELETE FROM students;";
//         char* errMsg = nullptr;
//         int rc = sqlite3_exec(db_, sqlDel, nullptr, nullptr, &errMsg);
//         if (rc != SQLITE_OK) {
//             std::cerr << "saveAllStudents DELETE error: " << errMsg << "\n";
//             sqlite3_free(errMsg);
//             // keep going anyway
//         }
//     }

//     const char* sql =
//         "INSERT INTO students (name, email, password) VALUES (?, ?, ?);";
//     sqlite3_stmt* stmt = nullptr;

//     if (sqlite3_prepare_v2(db_, sql, -1, &stmt, nullptr) != SQLITE_OK) {
//         std::cerr << "saveAllStudents prepare error: "
//                   << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     int count = 0;

//     for (const auto& pair : students) {
//         const std::string& email = pair.first;
//         Student* s = pair.second;
//         if (!s) continue;

//         sqlite3_reset(stmt);
//         sqlite3_clear_bindings(stmt);

//         sqlite3_bind_text(stmt, 1, s->get_name().c_str(),     -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmt, 2, s->get_email().c_str(),    -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmt, 3, s->get_password().c_str(), -1, SQLITE_TRANSIENT);

//         int rc = sqlite3_step(stmt);
//         if (rc != SQLITE_DONE) {
//             std::cerr << "saveAllStudents INSERT error for " << email
//                       << ": " << sqlite3_errmsg(db_) << "\n";
//         } else {
//             ++count;
//         }
//     }

//     sqlite3_finalize(stmt);

//     std::cout << "saveAllStudents: saved " << count << " students to DB.\n";
//     return true;
// }


// bool Database::saveAllTutors(const std::unordered_map<std::string, Tutor*>& tutors) {
//     if (!open()) return false;

//     std::cout << "[DB] saveAllTutors: saving " << tutors.size() << " tutors\n";

//     // Clear subjects first, then tutors (we'll repopulate them)
//     if (!execSQL(db_, "DELETE FROM subjects;")) {
//         std::cerr << "saveAllTutors: failed to clear subjects table.\n";
//         return false;
//     }
//     if (!execSQL(db_, "DELETE FROM tutors;")) {
//         std::cerr << "saveAllTutors: failed to clear tutors table.\n";
//         return false;
//     }

//     // ---------- insert into tutors ----------
//     const char* sqlTut =
//         "INSERT INTO tutors "
//         "(name, email, password, days, total_ratings, total_completed, total_matched) "
//         "VALUES (?, ?, ?, ?, ?, ?, ?);";

//     sqlite3_stmt* stmtTut = nullptr;

//     if (sqlite3_prepare_v2(db_, sqlTut, -1, &stmtTut, nullptr) != SQLITE_OK) {
//         std::cerr << "saveAllTutors prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     for (const auto& pair : tutors) {
//         const Tutor* t = pair.second;

//         sqlite3_reset(stmtTut);
//         sqlite3_clear_bindings(stmtTut);

//         std::string daysStr = encodeDays(t->get_days());

//         sqlite3_bind_text(stmtTut, 1, t->get_name().c_str(),        -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmtTut, 2, t->get_email().c_str(),       -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmtTut, 3, t->get_password().c_str(),    -1, SQLITE_TRANSIENT);
//         sqlite3_bind_text(stmtTut, 4, daysStr.c_str(),              -1, SQLITE_TRANSIENT);

//         // Here I'm assuming:
//         //   - total_ratings  ~= t->avg_rating() * t->get_completed()
//         //   - total_completed = t->get_completed()
//         //   - total_matched   = t->get_matched()
//         //
//         // If you have a better way (e.g. a set_totals / get_totals), plug it in.

//         double avg   = t->avg_rating();
//         int completed = t->get_completed();
//         double total_ratings = avg * completed;

//         sqlite3_bind_double(stmtTut, 5, total_ratings);
//         sqlite3_bind_int   (stmtTut, 6, completed);
//         sqlite3_bind_int   (stmtTut, 7, t->get_matched());

//         int rc = sqlite3_step(stmtTut);
//         if (rc != SQLITE_DONE) {
//             std::cerr << "saveAllTutors tutor step error for email "
//                       << t->get_email() << ": "
//                       << sqlite3_errmsg(db_) << "\n";
//         }
//     }

//     sqlite3_finalize(stmtTut);

//     // ---------- insert into subjects ----------
//     const char* sqlSubj =
//         "INSERT INTO subjects (tutor_email, subject) VALUES (?, ?);";

//     sqlite3_stmt* stmtSubj = nullptr;

//     if (sqlite3_prepare_v2(db_, sqlSubj, -1, &stmtSubj, nullptr) != SQLITE_OK) {
//         std::cerr << "saveAllTutors subjects prepare error: " << sqlite3_errmsg(db_) << "\n";
//         return false;
//     }

//     for (const auto& pair : tutors) {
//         const Tutor* t = pair.second;
//         const auto& subs = t->get_subjects();

//         for (const std::string& subj : subs) {
//             sqlite3_reset(stmtSubj);
//             sqlite3_clear_bindings(stmtSubj);

//             sqlite3_bind_text(stmtSubj, 1, t->get_email().c_str(), -1, SQLITE_TRANSIENT);
//             sqlite3_bind_text(stmtSubj, 2, subj.c_str(),          -1, SQLITE_TRANSIENT);

//             int rc = sqlite3_step(stmtSubj);
//             if (rc != SQLITE_DONE) {
//                 std::cerr << "saveAllTutors subject step error for tutor "
//                           << t->get_email() << ": "
//                           << sqlite3_errmsg(db_) << "\n";
//             }
//         }
//     }

//     sqlite3_finalize(stmtSubj);
//     std::cout << "[DB] saveAllTutors: finished.\n";
//     return true;
// }
