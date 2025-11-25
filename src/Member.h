#ifndef MEMBER_H
#define MEMBER_H

#include "Person.h"

class Member : public Person {
public:
    // Simple Constructor
    Member(int id, string name) : Person(id, name) {}

    void displayInfo() override {
        cout << "[Member] ID: " << id << " | Name: " << name << endl;
    }
};

#endif