#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>

#include "../services/ConsoleUI.h"
#include "../entities/Admin.h"
#include "../entities/Member.h"
#include "../entities/Trainer.h"
#include "../services/AdminService.h"
#include "../services/MemberService.h"
#include "../services/TrainerService.h"

using namespace std;

class System
{
private:
    Admin *currentAdmin;           // Nullable admin pointer
    AdminService adminService;     // Service for admins
    MemberService memberService;   // Service for members
    TrainerService trainerService; // Service for trainers

public:
    // Constructor
    System() : currentAdmin(nullptr)
    {
        // Database classes initialize themselves with test data
    }

    // Destructor
    ~System()
    {
        if (currentAdmin != nullptr)
        {
            delete currentAdmin;
            currentAdmin = nullptr;
        }
        // Note: Database classes manage their own memory
    }

    // ==================== AUTHENTICATION ====================

    // Show authentication menu
    void showAuthMenu()
    {
        vector<string> options = {
            "Login",
            "About"};

        ConsoleUI::printMenu("El-Forma Gym Management - Authentication", options);
    }

    // Login function
    bool login()
    {
        currentAdmin = adminService.login();
        return currentAdmin != nullptr;
    }

    // Logout function
    void logout()
    {
        adminService.logout(currentAdmin);
        if (currentAdmin != nullptr)
        {
            delete currentAdmin;
            currentAdmin = nullptr;
        }
    }

    // Check if admin is logged in
    bool isLoggedIn() const
    {
        return currentAdmin != nullptr;
    }

    // Get current admin
    Admin *getCurrentAdmin() const
    {
        return currentAdmin;
    }

    // ==================== MEMBER CRUD ====================

    // Create - Add new member
    void addMember()
    {
        memberService.addMember();
    }

    // Read - View all members
    void viewAllMembers()
    {
        memberService.viewAllMembers();
    }

    // Update - Update member information
    void updateMember()
    {
        memberService.updateMember();
    }

    // Delete - Remove a member
    void deleteMember()
    {
        memberService.deleteMember();
    }

    // ==================== TRAINER CRUD ====================

    // Create - Add new trainer
    void addTrainer()
    {
        trainerService.addTrainer();
    }

    // Read - View all trainers
    void viewAllTrainers()
    {
        trainerService.viewAllTrainers();
    }

    // View assigned members
    void viewAssignedMembers()
    {
        trainerService.viewAssignedMembers();
    }

    // Update - Update trainer information
    void updateTrainer()
    {
        trainerService.updateTrainer(memberService.getAllMembers());
    }

    // Delete - Remove a trainer
    void deleteTrainer()
    {
        trainerService.deleteTrainer();
    }

    // ==================== MENU SYSTEM ====================

    // Show main menu (after login)
    void showMainMenu()
    {
        if (!isLoggedIn())
        {
            ConsoleUI::printError("Please login first!");
            return;
        }

        vector<string> options = {
            "Manage Members",
            "Manage Trainers",
            "Logout"};

        ConsoleUI::printMenu("El-Forma Gym Management - Main Menu", options);
    }

    // Show members management menu
    void showMembersMenu()
    {
        vector<string> options = {
            "Add Member",
            "View All Members",
            "Update Member",
            "Delete Member"};

        ConsoleUI::printMenu("Members Management", options);
    }

    // Show trainers management menu
    void showTrainersMenu()
    {
        vector<string> options = {
            "Add Trainer",
            "View All Trainers",
            "View Assigned Members",
            "Update Trainer",
            "Delete Trainer"};

        ConsoleUI::printMenu("Trainers Management", options);
    }

    // Handle members menu
    void handleMembersMenu()
    {
        while (true)
        {
            showMembersMenu();
            int choice = ConsoleUI::getChoice();

            switch (choice)
            {
            case 0: // Back
                return;
            case 1: // Add
                addMember();
                ConsoleUI::pause();
                break;
            case 2: // View All
                viewAllMembers();
                ConsoleUI::pause();
                break;
            case 3: // Update
                updateMember();
                ConsoleUI::pause();
                break;
            case 4: // Delete
                deleteMember();
                ConsoleUI::pause();
                break;
            default:
                ConsoleUI::printError("Invalid choice!");
                ConsoleUI::pause();
                break;
            }
        }
    }

    // Handle trainers menu
    void handleTrainersMenu()
    {
        while (true)
        {
            showTrainersMenu();
            int choice = ConsoleUI::getChoice();

            switch (choice)
            {
            case 0: // Back
                return;
            case 1: // Add
                addTrainer();
                ConsoleUI::pause();
                break;
            case 2: // View All
                viewAllTrainers();
                ConsoleUI::pause();
                break;
            case 3: // View Assigned Members
                viewAssignedMembers();
                ConsoleUI::pause();
                break;
            case 4: // Update
                updateTrainer();
                ConsoleUI::pause();
                break;
            case 5: // Delete
                deleteTrainer();
                ConsoleUI::pause();
                break;
            default:
                ConsoleUI::printError("Invalid choice!");
                ConsoleUI::pause();
                break;
            }
        }
    }

    // Run the application
    void run()
    {
        while (true)
        {
            // Check if we need to show the "Welcome/Login" screen
            if (!isLoggedIn())
            {
                // Define the options
                vector<string> authOptions = {
                    "Login to System",
                    "About El-Forma",
                    "Exit"
                };

                // Show the Menu
                int choice = 0; 

                // Handle selection (Index 0, 1, 2)
                if (choice == 0) {
                    if (!login()) continue; // If login fails, restart loop
                } 
                else if (choice == 1) {
                    ConsoleUI::printInfo("El-Forma System v2.0 - ITI Project");
                    ConsoleUI::pause();
                }
                else {
                    return; // Exit the program
                }
            }
            // We are logged in! Show the Dashboard.
            else
            {
                vector<string> mainOptions = {
                    "Manage Members",
                    "Manage Trainers",
                    "Logout"};

                // get menu choice here
                int choice = 1; 

                // Handle selection (Index 0, 1, 2)
                switch (choice)
                {
                case 0:
                    handleMembersMenu();
                    break;
                case 1:
                    handleTrainersMenu();
                    break;
                case 2:
                    logout();
                    break;
                }
            }
        }
    }
};

#endif // SYSTEM_H
