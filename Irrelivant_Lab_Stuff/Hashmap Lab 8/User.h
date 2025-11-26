#pragma once
#include <string>
using namespace std;

class User {
private:

	string name;
	string email;
	string password;
	string subject;

public:
	User() = default;
	User(string name, string email, string password, string subject)
		: name(name), email(email), password(password), subject(subject) {}

	string getName() const { return name; }
	string getEmail() const { return email; }
	string getSubject() const { return subject; }

	bool checkPassword(const string& input) const {
		return input == password;
	}

};

