#ifndef ADMIN_SERVICE_H
#define ADMIN_SERVICE_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Admin.h"
#include "../services/ConsoleUI.h"

using namespace std;

// AdminService class - handles admin operations with UI
class AdminService {
private:
    // Vector to store admins in memory (shared across all instances)
    static vector<Admin*> admins;
    static bool initialized;
    
    // Initialize with default admin
    void initialize() {
        if (!initialized) {
            // Create default admin
            admins.push_back(new Admin("Mohamed Rashad", "mohamed@gmail.com", "admin"));
            initialized = true;
        }
    }

public:
    // Constructor
    AdminService() {
        initialize();
    }

    // Login with UI
    Admin* login() {
        ConsoleUI::printHeader("Admin Login");
        
        string email = ConsoleUI::getInput("Enter email: ");
        string password = ConsoleUI::getInput("Enter password: ");

        // Authenticate
        Admin* admin = authenticate(email, password);
        
        if (admin != nullptr) {
            ConsoleUI::printSuccess("Login successful!");
            ConsoleUI::pause();
            return admin;
        }
        
        ConsoleUI::printError("Invalid email or password!");
        ConsoleUI::pause();
        return nullptr;
    }
    
    // Logout with UI
    void logout(Admin* admin) {
        if (admin != nullptr) {
            admin->logout();
            ConsoleUI::printSuccess("Logged out successfully!");
        } else {
            ConsoleUI::printWarning("No user is currently logged in.");
        }
        ConsoleUI::pause();
    }
    
    // Authenticate admin (internal use)
    Admin* authenticate(const string& email, const string& password) {
        initialize();
        
        for (Admin* admin : admins) {
            if (admin->login(email, password)) {
                return admin;
            }
        }
        
        return nullptr;
    }
    
    // Find admin by email (internal use)
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
};

// Initialize static members
vector<Admin*> AdminService::admins;
bool AdminService::initialized = false;

#endif // ADMIN_SERVICE_H
