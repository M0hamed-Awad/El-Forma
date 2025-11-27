#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
using namespace std;

class User {
protected:
    int id;
    string name;

public:
    User(int id, string name) : id(id), name(name) {}

    int getId() const { return id; }
    string getName() const { return name; }

    // function to test the inheritance
    virtual void displayInfo() {
        cout << "ID: " << id << ", Name: " << name << endl;
    }
};

#endif