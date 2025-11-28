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

public:
    User(string userName, string userEmail, string userPassword)
        : name(userName), email(userEmail), password(userPassword), id(0)
    {
        // ID will be set by derived class
    }

    // Getters
    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    
    // Setters
    void setId(int newId) { id = newId; }

    virtual ~User() {}
};

#endif