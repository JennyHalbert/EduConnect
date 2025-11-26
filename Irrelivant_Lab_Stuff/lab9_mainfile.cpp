#include <iostream>
#include <vector>
#include <string>
#include <functional> // For std::function

// --- Forward Declaration ---
class Tutor;

// --- The Merge Logic ---
// Notice the last argument: 'Comparator comp'
template <typename Comparator>
void merge(std::vector<Tutor*>& tutors, int left, int mid, int right, Comparator comp) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    // Temporary vectors
    std::vector<Tutor*> L(n1);
    std::vector<Tutor*> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = tutors[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = tutors[mid + 1 + j];

    int i = 0; 
    int j = 0; 
    int k = left;

    while (i < n1 && j < n2) {
        // USE THE COMPARATOR HERE!
        // comp(a, b) should return true if 'a' belongs before 'b'
        if (comp(L[i], R[j])) {
            tutors[k] = L[i];
            i++;
        } else {
            tutors[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy remaining elements
    while (i < n1) {
        tutors[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        tutors[k] = R[j];
        j++;
        k++;
    }
}

// --- The Recursive Sort Logic ---
template <typename Comparator>
void mergeSort(std::vector<Tutor*>& tutors, int left, int right, Comparator comp) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Pass the 'comp' function down the recursion chain
        mergeSort(tutors, left, mid, comp);
        mergeSort(tutors, mid + 1, right, comp);
        merge(tutors, left, mid, right, comp);
    }
}

// --- Minimal Tutor Class ---
class Tutor {
public:
    std::string name;
    double rating;
    int completedSessions;

    Tutor(std::string n, double r, int c) 
        : name(n), rating(r), completedSessions(c) {}
    
    // Getters (Good practice)
    std::string getName() const { return name; }
    double getRating() const { return rating; }
    int getCompletedSessions() const { return completedSessions; }
};

// ... (Include the merge and mergeSort functions from above here) ...

int main() {
    // 1. Create Data
    std::vector<Tutor*> myTutors;
    myTutors.push_back(new Tutor("Alice", 3.5, 100));
    myTutors.push_back(new Tutor("Bob", 5.0, 20)); // High rating, low sessions
    myTutors.push_back(new Tutor("Charlie", 4.2, 50));
    myTutors.push_back(new Tutor("Dave", 5.0, 200)); // High rating, high sessions

    int n = myTutors.size();

    // ---------------------------------------------------------
    // SCENARIO A: Student wants "Highest Rated" first
    // ---------------------------------------------------------
    // Logic: return true if A > B (Descending)
    // Stability Check: Use >= to maintain stability for equal values
    mergeSort(myTutors, 0, n - 1, [](Tutor* a, Tutor* b) {
        return a->getRating() >= b->getRating(); 
    });

    std::cout << "--- Sorted by Rating (High -> Low) ---\n";
    for (auto t : myTutors) std::cout << t->name << ": " << t->rating << "\n";


    // ---------------------------------------------------------
    // SCENARIO B: Student wants "Most Experienced" first // the most amount of completed sessions
    // ---------------------------------------------------------
    // Logic: return true if A > B (Descending)
    mergeSort(myTutors, 0, n - 1, [](Tutor* a, Tutor* b) {
        return a->getCompletedSessions() >= b->getCompletedSessions(); 
    });

    std::cout << "\n--- Sorted by Experience (High -> Low) ---\n";
    for (auto t : myTutors) std::cout << t->name << ": " << t->completedSessions << "\n";


    // ---------------------------------------------------------
    // SCENARIO C: Student wants "Alphabetical"
    // ---------------------------------------------------------
    // Logic: return true if A < B (Ascending)
    mergeSort(myTutors, 0, n - 1, [](Tutor* a, Tutor* b) {
        return a->getName() <= b->getName(); 
    });

    // add sort by completion rate 

    // make sure days of the week are also displayed

    



    std::cout << "\n--- Sorted by Name (A -> Z) ---\n";
    for (auto t : myTutors) std::cout << t->name << "\n";

    // Cleanup
    for (auto t : myTutors) delete t;

    return 0;
}