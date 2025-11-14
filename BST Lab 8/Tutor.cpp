#include "Tutor.h"
#include "BST.h"

BST::BST() : root(nullptr) {}

Tutor::Tutor(string e, int completed, int matched)
	: email(e), totalCompleted(completed), totalMatched(matched), totalRating(0.0), completionRate(0.0) {
	if (totalCompleted > 0) avgRating = totalRating / totalCompleted;
	if (totalMatched > 0) completionRate = (double)totalCompleted / totalMatched;
}


void BST::insert(Node*& node, Tutor* t) {
	if (!node) {
		node = new Node(t);
		return;
	}
	if (t->avgRating < node->data->avgRating)
		insert(node->left, t);
	else
		insert(node->right, t);
}

void BST::insert(Tutor* t) {
	insert(root, t);
}


void BST::printInorder(Node* node) {
	if (!node) return;
	printInorder(node->right);
	cout << "Tutor: " << node->data->email << endl;
	cout << "Average rating: " << node->data->avgRating << endl;
	cout << "Completion Rate: " << node->data->completionRate << endl;
	cout << "-----------------" << endl;
	printInorder(node->left);
}


void BST::printInorder() {
	printInorder(root);
}

void BST::collectInorder(Node* node, vector<Tutor*>& tutors) {
	if (!node) return;
	collectInorder(node->left, tutors);
	tutors.push_back(node->data);
	collectInorder(node->right, tutors);
}

void BST::clear(Node* node) {
	if (!node) return;
	clear(node->left);
	clear(node->right);
	delete node;
}

void BST::rebuild() {
	vector<Tutor*> tutors;
	collectInorder(root, tutors);
	clear(root);
	root = nullptr;
	for (auto t : tutors)
		insert(t);
}

void Tutor::addRating(double newRating, BST& bst) {

	totalRating += newRating;
	totalCompleted++;
	avgRating = totalRating / totalCompleted;

	if (totalMatched > 0)
		completionRate = (double)totalCompleted / totalMatched;

	bst.rebuild();
}

void Tutor::completeSession() {
	if(totalMatched > 0)
	completionRate = (double)totalCompleted / totalMatched;
}