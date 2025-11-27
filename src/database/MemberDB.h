#ifndef MEMBER_DB_H
#define MEMBER_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../services/DB.h"
#include "../entities/Member.h"
#include "IDManager.h"

using namespace std;

// MemberDB class - handles member-specific database operations
class MemberDB {
private:
    DB db;

public:
    // Constructor
    MemberDB(const string& filename = "members.txt") : db(filename) {}

    // Load all members from file
    // Format: id,name,email,password,joinDate,subscriptionId
    vector<Member*> loadMembers() const {
        vector<Member*> members;
        vector<string> lines = db.readLines();
        int maxId = 0;

        for (const string& line : lines) {
            vector<string> fields = DB::split(line, ',');
            
            // Ensure we have at least 4 fields: id, name, email, password
            // Optional: joinDate (5th), subscriptionId (6th)
            if (fields.size() >= 4) {
                int id = 0;
                string name = DB::trim(fields[1]);
                string email = DB::trim(fields[2]);
                string password = DB::trim(fields[3]);
                
                // Parse ID
                try {
                    id = stoi(DB::trim(fields[0]));
                    if (id > maxId) maxId = id;
                } catch (...) {
                    continue; // Skip invalid ID
                }
                
                if (!name.empty() && !email.empty() && !password.empty()) {
                    Member* member;
                    
                    // Check if join date is provided
                    if (fields.size() >= 5 && !DB::trim(fields[4]).empty()) {
                        string joinDate = DB::trim(fields[4]);
                        member = new Member(name, email, password, joinDate);
                    } else {
                        member = new Member(name, email, password);
                    }
                    
                    // Restore the ID from file
                    member->setId(id);
                    
                    // Set subscription ID if provided
                    if (fields.size() >= 6 && !DB::trim(fields[5]).empty()) {
                        try {
                            int subId = stoi(DB::trim(fields[5]));
                            member->setSubscriptionId(subId);
                        } catch (...) {
                            // Invalid subscription ID, keep default 0
                        }
                    }
                    
                    members.push_back(member);
                }
            }
        }
        
        // Update the ID counter
        if (maxId > 0) {
            IDManager::saveLastID("Member", maxId);
        }

        return members;
    }

    // Save all members to file
    bool saveMembers(const vector<Member*>& members) const {
        vector<string> lines;
        int maxId = 0;
        
        for (const Member* member : members) {
            // Format: id,name,email,password,joinDate,subscriptionId
            string line = to_string(member->getId()) + "," +
                         member->getName() + "," + 
                         member->getEmail() + "," +
                         "******" + "," +  // Don't save actual password
                         member->getJoinDate() + "," +
                         to_string(member->getSubscriptionId());
            lines.push_back(line);
            
            if (member->getId() > maxId) {
                maxId = member->getId();
            }
        }
        
        // Update ID counter
        if (maxId > 0) {
            IDManager::saveLastID("Member", maxId);
        }

        return db.writeLines(lines);
    }

    // Add a new member
    bool addMember(const Member* member) const {
        string line = member->getName() + "," + 
                     member->getEmail() + "," +
                     "******" + "," +  // Password placeholder
                     member->getJoinDate() + "," +
                     to_string(member->getSubscriptionId());
        return db.appendLine(line);
    }

    // Update a member (replace entire file)
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
        vector<Member*> members = loadMembers();
        
        for (Member* member : members) {
            if (member->getId() == id) {
                // Clean up other members
                for (Member* m : members) {
                    if (m != member) {
                        delete m;
                    }
                }
                return member;
            }
        }
        
        // Clean up all members if not found
        for (Member* m : members) {
            delete m;
        }
        return nullptr;
    }

    // Find member by email
    Member* findMemberByEmail(const string& email) const {
        vector<Member*> members = loadMembers();
        
        for (Member* member : members) {
            if (member->getEmail() == email) {
                // Clean up other members
                for (Member* m : members) {
                    if (m != member) {
                        delete m;
                    }
                }
                return member;
            }
        }
        
        // Clean up all members if not found
        for (Member* m : members) {
            delete m;
        }
        return nullptr;
    }

    // Check if file exists
    bool fileExists() const {
        return db.fileExists();
    }

    // Get filename
    string getFilename() const {
        return db.getFilename();
    }
};

#endif // MEMBER_DB_H
