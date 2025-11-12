#include <iostream>
#include <unordered_map>

#include "Tutor.h"
#include "Request.h"
#include "User.h"
#include "Student.h"

using namespace std;

void processSessionCompletion(Request& session,unordered_map<string, Tutor>& tutorMap,int rating){
cout << "\nProcessing Session Completion (Rating: " << rating << ")\n";

// session completed as status c
session.update_status('C'); 

// tutor request from email
Tutor assignedTutor = session.get_tutor();         
std::string email = assignedTutor.get_email();     


// update tutor info 
auto it = tutorMap.find(email);
if (it == tutorMap.end()) {
cout << "Error: Tutor with email '" << email << "' not found in map.\n";
return;
}

// update the object stored inside the map
it->second.update_completed();
it->second.update_ratings(rating);

cout << "Tutor with '" << email << "' email information has been updated.\n";

}



int main()
{
    unordered_map<string, Tutor> tutormap; 

// hard coded values for testing     
    Tutor t1("tutor1@email.com","name1","pass1");
    Tutor t2("tutor2@email.com","name2","pass2");
    Tutor t3("tutor3@email.com","name3","pass3");

    Student s1("student1@email.com","studentname1","studentpass1");
    Student s2("student2@email.com","studentname2","studentpass2");
    Student s3("student3@email.com","studentname3","studentpass3");

    //removed description
    Request r1(t1, s1, "Math", 'M', 'H');
    Request r2(t2, s2, "English", 'E', 'L');
    Request r3(t3, s3, "Math", 'M', 'M');


// return tutor object
    tutormap[t1.get_email()] = t1; 
    tutormap[t2.get_email()] = t2;
    tutormap[t3.get_email()] = t3; 


// 
    processSessionCompletion(r1,tutormap,5);
    processSessionCompletion(r2,tutormap,3);
    processSessionCompletion(r3,tutormap,4);

// Print the key-value pairs
    for (auto it : tutormap)
    //print ratings
        cout << it.first << " " << it.second.avg_rating() << endl;

    return 0;
}

