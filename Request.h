#pragma once
#include <string>
#include "Student.h"
#include "Tutor.h"

<<<<<<< Updated upstream
class Request{
    private:
=======
enum class RequestStatus {
    PENDING,
    MATCHED,
    COMPLETED,
    CANCELLED
};

class Request {
protected:
>>>>>>> Stashed changes
    Student student;
    Tutor tutor;
    std::string subject;
    std::string description;
    char urgency;
    char status;
    bool is_accepted;

<<<<<<< Updated upstream

};
=======
public:
    Request();
    Request(Tutor tutor, Student student, std::string subject, char status, char urgency, std::string description = "");
    const Tutor& get_tutor() const;
    const Student& get_student() const;
    char get_status() const;
    char get_urgency() const;
    bool get_is_accepted() const;
    const std::string& get_description() const;
    void update_status(char updated_status);
    void update_is_accepted(bool accept);
};
>>>>>>> Stashed changes
