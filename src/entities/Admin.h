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

            // Functions
    bool login(string userInputEmail, string userInputPassword)
    {
        if (userInputEmail == email && userInputPassword == password)
        {
            cout << "Login Successful! Welcome, " << name << "." << endl;
            return true;
        }
        else
        {
            cout << "Invalid email or password." << endl;
            return false;
        }
    }

    void logout() { cout << "Goodbye, " << name << "!" << endl; }

    // Destructor
    ~Admin() override {}
};

#endif