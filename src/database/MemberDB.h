#ifndef MEMBER_DB_H
#define MEMBER_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Member.h"
#include "IDManager.h"

using namespace std;

// MemberDB class - handles member-specific database operations (in-memory)
class MemberDB {
private:
    // Static vector to store members in memory
    static vector<Member*> members;
    static bool initialized;
    
    // Initialize with fake members for testing
    static void initialize() {
        if (!initialized) {
            // Create fake members for testing
            Member* member1 = new Member("John Doe", "john@gmail.com", "password123", "2024-01-15");
            member1->setId(1);
            member1->setSubscriptionId(1);
            members.push_back(member1);
            
            Member* member2 = new Member("Sarah Johnson", "sarah@gmail.com", "password456", "2024-02-20");
            member2->setId(2);
            member2->setSubscriptionId(2);
            members.push_back(member2);
            
            Member* member3 = new Member("Mike Wilson", "mike@gmail.com", "password789", "2024-03-10");
            member3->setId(3);
            member3->setSubscriptionId(0); // No subscription
            members.push_back(member3);
            
            // Update ID counter
            IDManager::saveLastID("Member", 3);
            initialized = true;
        }
    }

public:
    // Constructor
    MemberDB() {
        initialize();
    }

    // Load all members (returns the in-memory vector)
    vector<Member*> loadMembers() const {
        initialize();
        return members;
    }

    // Save all members (updates the in-memory vector)
    bool saveMembers(const vector<Member*>& newMembers) const {
        members = newMembers;
        
        // Update ID counter based on members
        int maxId = 0;
        for (const Member* member : members) {
            if (member->getId() > maxId) {
                maxId = member->getId();
            }
        }
        if (maxId > 0) {
            IDManager::saveLastID("Member", maxId);
        }
        
        return true;
    }

    // Add a new member
    bool addMember(Member* member) const {
        members.push_back(member);
        
        // Update ID counter
        if (member->getId() > IDManager::getLastID("Member")) {
            IDManager::saveLastID("Member", member->getId());
        }
        
        return true;
    }

    // Update a member (replace entire vector)
    bool updateMember(const vector<Member*>& allMembers) const {
        return saveMembers(allMembers);
    }

    // Delete a member (save all except the one to delete)
    bool deleteMember(int memberId, const vector<Member*>& allMembers) const {
        vector<Member*> remainingMembers;
        
        for (Member* member : allMembers) {
            if (member->getId() != memberId) {
                remainingMembers.push_back(member);
            }
        }
        
        return saveMembers(remainingMembers);
    }

    // Find member by ID
    Member* findMemberById(int id) const {
        for (Member* member : members) {
            if (member->getId() == id) {
                return member;
            }
        }
        
        return nullptr;
    }

    // Find member by email
    Member* findMemberByEmail(const string& email) const {
        for (Member* member : members) {
            if (member->getEmail() == email) {
                return member;
            }
        }
        
        return nullptr;
    }

    // Check if data exists (always true for in-memory)
    bool fileExists() const {
        return true;
    }

    // Get filename (no longer relevant but kept for compatibility)
    string getFilename() const {
        return "in-memory";
    }
};

// Initialize static members
vector<Member*> MemberDB::members;
bool MemberDB::initialized = false;

#endif // MEMBER_DB_H
