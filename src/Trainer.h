#ifndef TRAINER_H
#define TRAINER_H

#include "User.h"

class Trainer : public User {
public:
    Trainer(int id, string name) : User(id, name) {}

    void displayInfo() override {
        cout << "[Trainer] ID: " << id << " | Name: " << name << endl;
    }
};

#endif