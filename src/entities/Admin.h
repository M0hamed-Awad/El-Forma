#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <vector>
#include "User.h"
#include "Member.h"
#include "Trainer.h"
using namespace std;

class Admin : public User {
public:
    // Constructor
    Admin(string userName, string userEmail, string userPassword)
        : User(userName, userEmail, userPassword) {}

    // Implement virtual method from User
    void manageAccount(int userId) override {
        cout << "Admin managing account for user ID: " << userId << endl;
        // Logic to manage account later
    }

    // Admin-specific functions
    void addMember() {
        cout << ">> SUCCESS: addMember() function called!" << endl;
        cout << ">> (Logic to add member later)" << endl;
    }

    void generateReport() {
        cout << ">> SUCCESS: generateReport() function called!" << endl;
        cout << ">> (Stats will appear here)" << endl;
    }

    // Destructor
    ~Admin() override {}
};

#endif