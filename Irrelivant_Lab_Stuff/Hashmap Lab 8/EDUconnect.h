#pragma once
#include <unordered_map>
#include "User.h"

using namespace std;

class Role {
private:

	unordered_map<string, User> students;
	unordered_map<string, User> tutors;

public:

	void addStudent(const User& student);
	void addTutor(const User& tutor);

	User* getStudent(const string& email);
	User* getTutor(const string& email);

	User* matchTutor(const string& subject);

	bool authenticateStudent(const string& email, const string& password);
	bool authenticateTutor(const string& email, const string& password);
};
