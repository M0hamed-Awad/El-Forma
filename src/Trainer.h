#ifndef TRAINER_H
#define TRAINER_H

#include "User.h"
#include "Member.h"
#include <vector>

class Trainer : public User
{
    string specialty;
    vector<Member *> assignedMembers;

public:
    Trainer(string trainerName, string trainerEmail, string trainerPassword, string trainerSpecialty)
        : User(trainerName, trainerEmail, trainerPassword)
    {
        specialty = trainerSpecialty;
    }

    // Getters
    string getTrainerSpecialty() { return specialty; }
    vector<Member *> getAssignedMembers() { return assignedMembers; }

    // Setters
    void setTrainerSpecialty(string trainerSpecialty) { specialty = trainerSpecialty; }

    // Functions
    void manageAccount(int userId) override
    {
        cout << "Trainer " << name << " Profile | ID: " << id << endl;
        cout << "Specialty: " << specialty << endl;
    }

    void viewAssignedMembers()
    {
        cout << "Trainer " << this->name << " assigned members are:" << endl;
        for (Member *member : assignedMembers)
        {
            cout << "Member ID: " << member->getId() << ", Name: " << member->getName() << endl;
        }
    }

    void assignMember(Member *member)
    {
        // Each Trainer can have only up to 7 members assigned at a time.
        if (assignedMembers.size() == 7)
        {
            cout << ">> Trainer can only Manage 7 Members at a time!" << endl;
        }
        else
        {
            assignedMembers.push_back(member);
            cout << ">> System: " << member->getName() << " assigned to " << this->name << endl;
        }
    }
};

#endif