#pragma once
#include "Tutor.h"


class BST {
private:
	struct Node {
		Tutor* data;
		Node* left;
		Node* right;
		Node(Tutor* t) : data(t), left(nullptr), right(nullptr) {}
	};
	Node* root;

	void insert(Node*& node, Tutor* t);
	void printInorder(Node* node);
	void collectInorder(Node* node, vector<Tutor*>& tutors);
	void clear(Node* node);

public:
	BST();
	void insert(Tutor* t);
	void printInorder();
	void rebuild();
};
