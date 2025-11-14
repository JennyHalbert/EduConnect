#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class BST;

class Tutor {
public:
	string email;
	double totalRating;
	int totalCompleted;
	int totalMatched;
	double avgRating;
	double completionRate;

	Tutor(string e, int completed, int matched);

	void addRating(double newRating, BST& bst);
	void completeSession();
};



