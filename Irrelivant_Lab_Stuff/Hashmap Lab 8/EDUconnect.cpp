#include "EDUconnect.h"

void Role::addStudent(const User& student) {
	students[student.getEmail()] = student;
}

void Role::addTutor(const User& tutor) {
	tutors[tutor.getEmail()] = tutor;
}

User* Role::getStudent(const string& email) {
	auto it = students.find(email);
	if (it != students.end()) {
		return &it->second;
	}
	else {
		return nullptr;
	}
}

User* Role::getTutor(const string& email) {
	auto it = tutors.find(email);
	if (it != tutors.end()) {
		return &it->second;
	}
	else {
		return nullptr;
	}
}

User* Role::matchTutor(const string& subject) {
	for (auto& pair : tutors) {
		User& tutor = pair.second;
		if (tutor.getSubject() == subject) {
			return &tutor;
		}
	}
	return nullptr;
}

bool Role::authenticateStudent(const string& email, const string& password) {
	User* s = getStudent(email);
	return s && s->checkPassword(password);
}

bool Role::authenticateTutor(const string& email, const string& password) {
	User* t = getTutor(email);
	return t && t->checkPassword(password);
}
