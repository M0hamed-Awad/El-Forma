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

    // Handle members menu
    void handleMembersMenu()
    {
        while (true)
        {
            // list of actions for Members
            vector<string> opts = {
                "Add New Member",
                "View All Members",
                "Update Member",
                "Delete Member",
                "Back to Dashboard"};

            // Show the menu here
            int choice = ConsoleUI::getMenuSelection("MEMBERS MANAGEMENT", opts);
            if (choice == -1)
                return; // ESC pressed -> Go back

            switch (choice) {
                case 0: memberService.addMember(); break;
                case 1: memberService.viewAllMembers(); break;
                case 2: memberService.updateMember(); break;
                case 3: memberService.deleteMember(); break;
                case 4: return;
            }
        }
    }

    // Handle trainers menu
    void handleTrainersMenu()
    {
        while (true)
        {
            // list of actions for Members
            vector<string> opts = {
                "Add New Trainer",
                "View All Trainers",
                "View Assigned Members",
                "Update Trainer",
                "Delete Trainer",
                "Back to Dashboard"};

            int choice = ConsoleUI::getMenuSelection("TRAINERS MANAGEMENT", opts);
            if (choice == -1)
                return; // ESC pressed -> Go back

            switch (choice) {
                case 0: trainerService.addTrainer(); break;
                case 1: trainerService.viewAllTrainers(); break;
                case 2: trainerService.viewAssignedMembers(); break;
                case 3: trainerService.updateTrainer(memberService.getAllMembers()); break;
                case 4: trainerService.deleteTrainer(); break;
                case 5: return;
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
                    "Login",
                    "About El-Forma",
                    "Exit"};

                // Show the Menu
                int choice = ConsoleUI::getMenuSelection("WELCOME TO EL-FORMA", authOptions);

                // Handle selection (Index 0, 1, 2)
                // Login
                if (choice == 0)
                {
                    if (!login())
                        continue; // If login fails, restart loop
                }
                // About
                else if (choice == 1)
                {
                    ConsoleUI::printInfo("El-Forma System v2.0 - ITI Project");
                    ConsoleUI::pause();
                }
                else if (choice == -1)
                { // ESC pressed -> Logout
                    logout();
                    continue; // Restart loop to show Welcome Screen
                }
                else
                {
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
                int choice = ConsoleUI::getMenuSelection("MAIN DASHBOARD", mainOptions);

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

#endif
