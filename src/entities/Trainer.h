#ifndef TRAINER_H
#define TRAINER_H

#include "User.h"
#include "Member.h"
#include <vector>

class Trainer : public User
{
    string specialty;
    vector<Member *> assignedMembers;
    
    inline static int nextTrainerId = 0;
    inline static bool loadingFromDB = false;

public:
    Trainer(string trainerName, string trainerEmail, string trainerPassword, string trainerSpecialty)
        : User(trainerName, trainerEmail, trainerPassword)
    {
        specialty = trainerSpecialty;
        
        if (!loadingFromDB) {
            nextTrainerId++;
            id = nextTrainerId;
        }
    }

    // Getters
    string getTrainerSpecialty() const { return specialty; }
    vector<Member *> getAssignedMembers() const { return assignedMembers; }
    
    // Static ID management
    static void setNextTrainerId(int lastId) { nextTrainerId = lastId; }
    static void setLoadingMode(bool loading) { loadingFromDB = loading; }

    // Setters
    void setTrainerSpecialty(string trainerSpecialty) { specialty = trainerSpecialty; }

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

    // Remove a member from this trainer's list by ID
    void removeMember(int memberId)
    {
        for (auto it = assignedMembers.begin(); it != assignedMembers.end(); ++it)
        {
            if ((*it)->getId() == memberId)
            {
                // We only erase the POINTER from this list.
                // We do NOT 'delete *it' here, because MemberService is about to do that.
                assignedMembers.erase(it); 
                cout << ">> System: Member removed from Trainer " << this->name << "'s list." << endl;
                return;
            }
        }
    }
};

#endif