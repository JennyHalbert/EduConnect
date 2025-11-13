#include <iostream>
#include <queue>
#include <vector>
#include "Request.h"
#include "Tutor.h"
#include "Student.h"

using namespace std;

struct CompareRequestUrgency {//define a structure to handle the logic for the priority queue comparisons
    bool operator()(Request* a, Request* b) {
        // 'true' means 'a' has lower priority than 'b'
        return a->get_urgency() < b->get_urgency();
    }
};

int main(){

// Define tutors
    Tutor t1("tutor1@email.com","name1","pass1");
    Tutor t2("tutor2@email.com","name2","pass2");
    Tutor t3("tutor3@email.com","name3","pass3");

// define students
    Student s1("student1@email.com","studentname1","studentpass1");
    Student s2("student2@email.com","studentname2","studentpass2");
    Student s3("student3@email.com","studentname3","studentpass3");
    

    Request* r1 = new Request(t1, s1, "Math", PENDING, HIGH);
    Request* r2 = new Request(t2, s2, "English", PENDING, LOW);
    Request* r3 = new Request(t3, s3, "Math", PENDING, LOW);

 // Create a priority queue of request pointers, define how the priority queue stores data, and define how the priority queue pointers will be compared
priority_queue<Request*,vector<Request*>,CompareRequestUrgency> pending_request;
    // Add some elements to the priority queue
    cout << "Students submitting requests\n";
    pending_request.push(r1);
    pending_request.push(r2);
    pending_request.push(r3);

     cout << "Total pending requests: " << pending_request.size() << "\n\n";

    // Display and remove elements from the priority queue
    cout << "Priority Queue elements in order:" << endl;
    while (!pending_request.empty()) {
        // Get the highest-priority item
        Request* topRequest = pending_request.top();

  
        cout << "  Processing: [" << topRequest->get_urgency() << "] " 
                  << topRequest->get_subject() << "\n";

        // Remove it from the queue
        pending_request.pop();
        
        delete topRequest; 
    }
    cout << endl;
 
    return 0;
}



