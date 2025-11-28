#ifndef MEMBER_DB_H
#define MEMBER_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Member.h"

using namespace std;

// MemberDB class - handles member-specific database operations (in-memory)
class MemberDB {
private:
    // Vector to store members in memory (shared across all instances)
    static vector<Member*> members;
    static bool initialized;
    
    // Initialize with fake members for testing
    void initialize() {
        if (!initialized) {
            // Create fake members using add method (IDs auto-assigned)
            addMember(new Member("John Doe", "john@gmail.com", "password123", "2024-01-15"));
            members.back()->setSubscriptionId(1);
            
            addMember(new Member("Sarah Johnson", "sarah@gmail.com", "password456", "2024-02-20"));
            members.back()->setSubscriptionId(2);
            
            addMember(new Member("Mike Wilson", "mike@gmail.com", "password789", "2024-03-10"));
            members.back()->setSubscriptionId(0); // No subscription
            
            initialized = true;
        }
    }

public:
    // Constructor
    MemberDB() {
        initialize();
    }

    // Load all members (returns the in-memory vector)
    vector<Member*> loadMembers() {
        initialize();
        return members;
    }

    // Save all members (updates the in-memory vector)
    bool saveMembers(const vector<Member*>& newMembers) {
        members = newMembers;
        return true;
    }

    // Add a new member
    bool addMember(Member* member) {
        if (!initialized) {
            // During initialization, just add without calling initialize()
            members.push_back(member);
        } else {
            initialize();
            members.push_back(member);
        }
        return true;
    }

    // Find member by ID
    Member* findMemberById(int id) {
        initialize();
        for (Member* member : members) {
            if (member->getId() == id) {
                return member;
            }
        }
        return nullptr;
    }

    // Find member by email
    Member* findMemberByEmail(const string& email) {
        initialize();
        for (Member* member : members) {
            if (member->getEmail() == email) {
                return member;
            }
        }
        return nullptr;
    }
};

// Initialize static members
vector<Member*> MemberDB::members;
bool MemberDB::initialized = false;

#endif // MEMBER_DB_H
