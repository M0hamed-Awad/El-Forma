#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>

#include "../services/ConsoleUI.h"
#include "../entities/Admin.h"
#include "../database/AdminDB.h"


using namespace std;

class System {
private:
    Admin* currentAdmin;  // Nullable admin pointer

public:
    // Constructor
    System() : currentAdmin(nullptr) {}

    // Destructor
    ~System() {
        if (currentAdmin != nullptr) {
            delete currentAdmin;
            currentAdmin = nullptr;
        }
    }

    // Show authentication menu
    void showAuthMenu() {
        vector<string> options = {
            "Login",
            "About"
        };
        
        ConsoleUI::printMenu("El-Forma Gym Management - Authentication", options);
    }

    // Login function
    bool login() {
        ConsoleUI::printHeader("Admin Login");
        
        string email = ConsoleUI::getInput("Enter email: ");
        string password = ConsoleUI::getInput("Enter password: ");

        // Use AdminDB to authenticate
        AdminDB adminDB;
        currentAdmin = adminDB.authenticate(email, password);
        
        if (currentAdmin != nullptr) {
            ConsoleUI::printSuccess("Login successful!");
            ConsoleUI::pause();
            return true;
        }
        
        ConsoleUI::printError("Invalid email or password!");
        ConsoleUI::pause();
        return false;
    }

    // Logout function
    void logout() {
        if (currentAdmin != nullptr) {
            currentAdmin->logout();
            delete currentAdmin;
            currentAdmin = nullptr;
            ConsoleUI::printSuccess("Logged out successfully!");
        }
        else {
            ConsoleUI::printWarning("No user is currently logged in.");
        }
        ConsoleUI::pause();
    }

    // Check if admin is logged in
    bool isLoggedIn() const {
        return currentAdmin != nullptr;
    }

    // Get current admin
    Admin* getCurrentAdmin() const {
        return currentAdmin;
    }

    // Show main menu (after login)
    void showMainMenu() {
        if (!isLoggedIn()) {
            ConsoleUI::printError("Please login first!");
            return;
        }

        vector<string> options = {
            "Add Member",
            "Generate Report",
            "Manage Account",
            "Logout"
        };
        
        ConsoleUI::printMenu("El-Forma Gym Management - Main Menu", options);
    }

    // Run the application
    void run() {
        while (true) {
            if (!isLoggedIn()) {
                // Show authentication menu
                showAuthMenu();
                int choice = ConsoleUI::getChoice();

                switch (choice) {
                    case 0: // Exit
                        ConsoleUI::printInfo("Goodbye!");
                        return;
                    case 1: // Login
                        login();
                        break;
                    case 2: // About
                        ConsoleUI::printHeader("About El-Forma");
                        ConsoleUI::printInfo("El-Forma Gym Management System v1.0");
                        ConsoleUI::printInfo("Developed for ITI SWE Course");
                        ConsoleUI::pause();
                        break;
                    default:
                        ConsoleUI::printError("Invalid choice!");
                        ConsoleUI::pause();
                        break;
                }
            }
            else {
                // Show main menu (logged in)
                showMainMenu();
                int choice = ConsoleUI::getChoice();

                switch (choice) {
                    case 0: // Back/Logout
                        logout();
                        break;
                    case 1: // Add Member
                        currentAdmin->addMember();
                        ConsoleUI::pause();
                        break;
                    case 2: // Generate Report
                        currentAdmin->generateReport();
                        ConsoleUI::pause();
                        break;
                    case 3: // Manage Account
                        currentAdmin->manageAccount(currentAdmin->getId());
                        ConsoleUI::pause();
                        break;
                    case 4: // Logout
                        logout();
                        break;
                    default:
                        ConsoleUI::printError("Invalid choice!");
                        ConsoleUI::pause();
                        break;
                }
            }
        }
    }
};

#endif // SYSTEM_H
