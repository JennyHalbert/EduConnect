#include <iostream>
#include "EDUconnect.h"
using namespace std;


void findMatch(Role& platform, string& email) {

	User* student = platform.getStudent(email);

	if (student) {
		string studentSubject = student->getSubject();

		cout << "Matching tutor for " << student->getName() << "'s subject (" << studentSubject << "): " << endl;
		User* tutor = platform.matchTutor(studentSubject);

		if (tutor)
			cout << "Matched with: " << tutor->getName() << " (" << tutor->getEmail() << ")" << endl;
		else
			cout << "No tutor found. " << endl;
	}
	else {
		cout << "Student not found. " << endl;
	}
}

void authentication(Role& platform, string& email, string& password) {

	User* student = platform.getStudent(email);
	
	cout << "Authentication Test: " << endl;
	if (platform.authenticateStudent(email, password))
		cout << "Login successful for " << student -> getName() << endl;
	else
		cout << "Login failed for " << student->getName() << endl;

}

int main() {

	Role platform;

	platform.addStudent(User("Clay", "onischuk18@gmail.com", "password345", "Science"));
	platform.addStudent(User("Thomas", "Thomas48@gmail.com", "Pass12", "Math"));
	platform.addStudent(User("Jenny", "Jenny1234@gmail.com", "Password91", "Physics"));

	platform.addTutor(User("John", "John@hotmail.com", "Tutor456", "Math"));
	platform.addTutor(User("Alice", "Alice@hotmail.com", "Alice789", "Physics"));
	platform.addTutor(User("Bob", "Bob@hotmail.com", "Bob842", "Science"));


	string studentEmail1 = "onischuk18@gmail.com";
	string studentEmail2 = "Thomas48@gmail.com";
	string studentEmail3 = "Jenny1234@gmail.com";

	findMatch(platform, studentEmail1);
	findMatch(platform, studentEmail2);
	findMatch(platform, studentEmail3);

	string Student1email = "Thomas48@gmail.com";
	string Student1password = "Pass12";

	string Student2email = "Jenny1234@gmail.com";
	string Student2password = "Password67";

	authentication(platform, Student1email, Student1password);
	authentication(platform, Student2email, Student2password);
		
	return 0;
}
