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
    // Static vector to store admins in memory
    static vector<Admin*> admins;
    static bool initialized;
    
    // Initialize with default admin
    static void initialize() {
        if (!initialized) {
            // Create default admin: admin@gmail.com / admin
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
    vector<Admin*> loadAdmins() const {
        initialize();
        return admins;
    }

    // Save all admins (updates the in-memory vector)
    bool saveAdmins(const vector<Admin*>& newAdmins) const {
        // Clear old admins (don't delete, assume caller manages memory)
        admins = newAdmins;
        return true;
    }

    // Find admin by email
    Admin* findAdminByEmail(const string& email) const {
        initialize();
        
        for (Admin* admin : admins) {
            if (admin->getEmail() == email) {
                return admin;
            }
        }
        
        return nullptr;
    }

    // Add a new admin
    bool addAdmin(Admin* admin) const {
        initialize();
        admins.push_back(admin);
        return true;
    }

    // Authenticate admin
    Admin* authenticate(const string& email, const string& password) const {
        initialize();
        
        for (Admin* admin : admins) {
            if (admin->login(email, password)) {
                return admin;
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
vector<Admin*> AdminDB::admins;
bool AdminDB::initialized = false;

#endif // ADMIN_DB_H
