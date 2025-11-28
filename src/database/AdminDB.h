#ifndef ADMIN_DB_H
#define ADMIN_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Admin.h"

using namespace std;

// AdminDB class - handles admin-specific database operations (in-memory)
class AdminDB {
private:
    // Vector to store admins in memory (shared across all instances)
    static vector<Admin*> admins;
    static bool initialized;
    
    // Initialize with default admin
    void initialize() {
        if (!initialized) {
            // Create default admin using addAdmin method
            Admin* defaultAdmin = new Admin("Admin", "admin@gmail.com", "admin");
            admins.push_back(defaultAdmin);
            initialized = true;
        }
    }

public:
    // Constructor
    AdminDB() {
        initialize();
    }

    // Load all admins (returns the in-memory vector)
    vector<Admin*> loadAdmins() {
        initialize();
        return admins;
    }

    // Find admin by email
    Admin* findAdminByEmail(const string& email) {
        initialize();
        
        for (Admin* admin : admins) {
            if (admin->getEmail() == email) {
                return admin;
            }
        }
        
        return nullptr;
    }

    // Add a new admin
    bool addAdmin(Admin* admin) {
        initialize();
        admins.push_back(admin);
        return true;
    }

    // Authenticate admin
    Admin* authenticate(const string& email, const string& password) {
        initialize();
        
        for (Admin* admin : admins) {
            if (admin->login(email, password)) {
                return admin;
            }
        }
        
        return nullptr;
    }
};

// Initialize static members
vector<Admin*> AdminDB::admins;
bool AdminDB::initialized = false;

#endif // ADMIN_DB_H
