#include <string>
#include "User.h"

//std::unordered_map<std::string, User*> userDatabase;

    User::User(std::string email,std::string name,std::string password){
        this->email = email;
        this->name = name;
        this->password = password;
    }
    std::string User::get_password() const{
        return password;
    }
    std::string User::get_email() const{
        return email;
    }
    std::string User::get_name() const{
        return name;
    }
    
//     //User registration
//     //count asks how many entries with that email exist
//     //1 = exists already, 0 = does not exist.
//     void registerNewUser(User* newUser){
//         const std::string& email = newUser->get_email();
    
//         if (userDatabase.count(email) == 0){
//             userDatabase.emplace(email, newUser);
//             std::cout << "Registered new user: " << email << endl;
//         }
//         else {
//             std::cout << "Error: Email " << email << " already exists." << endl;
//         }
//     }

//     //Retrieval
//     User* getUserProfile(const std::string& email){
//         std::unordered_map<std::string, User*>::iterator it = userDatabase.find(email); // main search function, finds matching email in system
//         // or auto it = userDatabase.find(email);
//         // iterator it: pointer that points to a pair of data (key, value) in hashmap
//         if(it != userDatabase.end()) { // if it fails to find email it points to end marker
//             return it->second; //first element is email (key), second is the profile (value)
//         }
//         return nullptr; // returns nothing if iterator points to userDatabase.end() (user doesnt exist).
//     }

//     // NOT MY CODE, example for other opertions: Updating Tutor Subjects.
//     /*
//     // --- 3. Other Operation (Update Tutor Subjects - FR-1.3.2) ---
// // Complexity: O(1) average time (access)
// void updateTutorSubjects(const std::string& email, const std::vector<std::string>& newSubjects) {
//     User* user = getUserProfile(email);
    
//     // We use dynamic_cast because the HashMap only stores a generic User* pointer.
//     Tutor* tutor = dynamic_cast<Tutor*>(user); 
    
//     if (tutor != nullptr) {
//         tutor->setSubjects(newSubjects); 
//         std::cout << "✅ Updated subjects for Tutor " << tutor->get_email() << ".\n";
//     } else if (user != nullptr) {
//          std::cout << "❌ Error: User is not a Tutor.\n";
//     }
// }
//     */ 