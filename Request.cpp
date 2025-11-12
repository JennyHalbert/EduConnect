<<<<<<< Updated upstream
=======
#include <string>
#include <utility>
#include "Request.h"

Request::Request()
    : subject(""),
      description(""),
      urgency('L'),
      status('P'),
      is_accepted(false) {}

Request::Request(Tutor tutor, Student student, std::string subject, char status, char urgency, std::string description)
    : student(std::move(student)),
      tutor(std::move(tutor)),
      subject(std::move(subject)),
      description(std::move(description)),
      urgency(urgency),
      status(status),
      is_accepted(false) {}

const Tutor& Request::get_tutor() const {
    return tutor;
}

const Student& Request::get_student() const {
    return student;
}

char Request::get_status() const {
    return status;
}

char Request::get_urgency() const {
    return urgency;
}

bool Request::get_is_accepted() const {
    return is_accepted;
}

const std::string& Request::get_description() const {
    return description;
}

void Request::update_status(char updated_status) {
    status = updated_status;
}

void Request::update_is_accepted(bool accept) {
    is_accepted = accept;
}
>>>>>>> Stashed changes
