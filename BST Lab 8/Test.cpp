#include "Tutor.h"
#include "BST.h"

int main() {

	BST bst;
	//email, total completed, total matched
	// if tutor new start 0, 0
	Tutor* t1 = new Tutor("Thomas@gmail.com", 2, 5);
	Tutor* t2 = new Tutor("Jenny@gmail.com", 3, 6);
	Tutor* t3 = new Tutor("Clay@gmail.com", 1, 4);

	bst.insert(t1);
	bst.insert(t2);
	bst.insert(t3);

	cout << "Initial Tutors (Ratings: 0.0) " << endl;
	bst.printInorder();

	//update ratings
	t1->addRating(5.0, bst);
	t1->addRating(4.0, bst);
	t3->addRating(3.0, bst);
	

	cout << "-----------------------------------" << endl;
	cout << "Tutors sorted after new ratings: " << endl;
	bst.printInorder();


	
	delete t1;
	delete t2;
	delete t3;

	return 0;
}


/*
	//examples for updating rating
	cout << "Updating _____'s data." << endl;
	t1->updatedRating(5.0);

	//completes another session
	t1->completeSession();
	cout << "T1 new completion rate: " << t1->completionRate << endl;

	cout << "Tutor rating after updated data: " << endl;
	bst.printInorder();

	cou
	*/