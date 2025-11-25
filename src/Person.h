#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    int id;
    string name;

public:
    Person(int id, string name) : id(id), name(name) {}

    // function to test the inheritance
    virtual void displayInfo() {
        cout << "ID: " << id << ", Name: " << name << endl;
    }
};

#endif