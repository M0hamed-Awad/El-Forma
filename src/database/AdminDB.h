#ifndef ADMIN_DB_H
#define ADMIN_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../services/DB.h"
#include "../entities/Admin.h"

using namespace std;

// AdminDB class - handles admin-specific database operations
class AdminDB {
private:
    DB db;

public:
    // Constructor
    AdminDB(const string& filename = "admins.txt") : db(filename) {}

    // Load all admins from file
    // Format: name,email,password
    vector<Admin*> loadAdmins() const {
        vector<Admin*> admins;
        vector<string> lines = db.readLines();

        for (const string& line : lines) {
            vector<string> fields = DB::split(line, ',');
            
            // Ensure we have exactly 3 fields: name, email, password
            if (fields.size() >= 3) {
                string name = DB::trim(fields[0]);
                string email = DB::trim(fields[1]);
                string password = DB::trim(fields[2]);
                
                if (!name.empty() && !email.empty() && !password.empty()) {
                    Admin* admin = new Admin(name, email, password);
                    admins.push_back(admin);
                }
            }
        }

        return admins;
    }

    // Save all admins to file
    bool saveAdmins(const vector<Admin*>& admins) const {
        vector<string> lines;
        
        for (const Admin* admin : admins) {
            // Format: name,email,password
            string line = admin->getName() + "," + 
                         admin->getEmail() + "," +
                         "******";  // Don't save actual password for security
            lines.push_back(line);
        }

        return db.writeLines(lines);
    }

    // Find admin by email
    Admin* findAdminByEmail(const string& email) const {
        vector<Admin*> admins = loadAdmins();
        
        for (Admin* admin : admins) {
            if (admin->getEmail() == email) {
                // Clean up other admins
                for (Admin* a : admins) {
                    if (a != admin) {
                        delete a;
                    }
                }
                return admin;
            }
        }
        
        // Clean up all admins if not found
        for (Admin* a : admins) {
            delete a;
        }
        return nullptr;
    }

    // Add a new admin
    bool addAdmin(const Admin* admin) const {
        string line = admin->getName() + "," + 
                     admin->getEmail() + "," +
                     "******";  // Password placeholder
        return db.appendLine(line);
    }

    // Authenticate admin
    Admin* authenticate(const string& email, const string& password) const {
        vector<Admin*> admins = loadAdmins();
        
        for (Admin* admin : admins) {
            if (admin->login(email, password)) {
                // Clean up other admins
                for (Admin* a : admins) {
                    if (a != admin) {
                        delete a;
                    }
                }
                return admin;
            }
        }
        
        // Clean up all admins if authentication failed
        for (Admin* a : admins) {
            delete a;
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

#endif // ADMIN_DB_H
