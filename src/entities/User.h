#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User
{
protected:
    int id;
    string name;
    string email;
    string password;
    inline static int nextUserId = 0;

public:
    User(string userName, string userEmail, string userPassword)
        : name(userName), email(userEmail), password(userPassword)
    {
        nextUserId++;
        id = nextUserId;
    }

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }

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

    virtual void manageAccount(int userId) = 0;

    virtual ~User() {}
};

#endif