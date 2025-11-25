#ifndef TRAINER_H
#define TRAINER_H

#include "Person.h"

class Trainer : public Person {
public:
    Trainer(int id, string name) : Person(id, name) {}

    void displayInfo() override {
        cout << "[Trainer] ID: " << id << " | Name: " << name << endl;
    }
};

#endif