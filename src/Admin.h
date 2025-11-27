#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <vector>
#include "Member.h"
#include "Trainer.h"
using namespace std;

class Admin {
public:
    // dummy functions
    void addMember() {
        cout << ">> SUCCESS: addMember() function called!" << endl;
        cout << ">> (Logic to add member later)" << endl;
    }

    void generateReport() {
        cout << ">> SUCCESS: generateReport() function called!" << endl;
        cout << ">> (Stats will appear here)" << endl;
    }
};

#endif