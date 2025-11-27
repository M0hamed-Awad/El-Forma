#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <vector>

#include "../services/ConsoleUI.h"
#include "../entities/Admin.h"
#include "../entities/Member.h"
#include "../entities/Trainer.h"
#include "../database/AdminDB.h"
#include "../database/MemberDB.h"
#include "../database/TrainerDB.h"
#include "../database/IDManager.h"


using namespace std;

class System {
private:
    Admin* currentAdmin;  // Nullable admin pointer
    vector<Member*> members;  // List of members
    vector<Trainer*> trainers;  // List of trainers (coaches)

public:
    // Constructor
    System() : currentAdmin(nullptr) {
        // Load members and trainers from database first
        loadMembersFromDB();
        loadTrainersFromDB();
        
        // Initialize separate ID counters for each entity type
        int maxMemberId = IDManager::getLastID("Member");
        int maxTrainerId = IDManager::getLastID("Trainer");
        
        Member::setNextMemberId(maxMemberId);
        Trainer::setNextTrainerId(maxTrainerId);
    }

    // Destructor
    ~System() {
        // Save data before cleanup
        saveMembersToDB();
        saveTrainersToDB();
        
        if (currentAdmin != nullptr) {
            delete currentAdmin;
            currentAdmin = nullptr;
        }
        
        // Clean up members
        for (Member* member : members) {
            delete member;
        }
        members.clear();
        
        // Clean up trainers
        for (Trainer* trainer : trainers) {
            delete trainer;
        }
        trainers.clear();
    }

    // ==================== AUTHENTICATION ====================
    
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

    // ==================== DATABASE PERSISTENCE ====================
    
    // Load members from database
    void loadMembersFromDB() {
        Member::setLoadingMode(true);  // Disable auto-increment during load
        MemberDB memberDB;
        members = memberDB.loadMembers();
        Member::setLoadingMode(false);  // Re-enable auto-increment
    }
    
    // Save members to database
    void saveMembersToDB() {
        MemberDB memberDB;
        memberDB.saveMembers(members);
    }
    
    // Load trainers from database
    void loadTrainersFromDB() {
        Trainer::setLoadingMode(true);  // Disable auto-increment during load
        TrainerDB trainerDB;
        trainers = trainerDB.loadTrainers();
        Trainer::setLoadingMode(false);  // Re-enable auto-increment
    }
    
    // Save trainers to database
    void saveTrainersToDB() {
        TrainerDB trainerDB;
        trainerDB.saveTrainers(trainers);
    }

    // ==================== MEMBER CRUD ====================
    
    // Create - Add new member
    void addMember() {
        ConsoleUI::printHeader("Add New Member");
        
        string name = ConsoleUI::getInput("Enter member name: ");
        string email = ConsoleUI::getInput("Enter member email: ");
        string password = ConsoleUI::getInput("Enter member password: ");
        
        Member* newMember = new Member(name, email, password);
        members.push_back(newMember);
        
        // Save to database
        saveMembersToDB();
        
        ConsoleUI::printSuccess("Member added successfully!");
        ConsoleUI::printInfo("Member ID: " + to_string(newMember->getId()));
        ConsoleUI::printInfo("Join Date: " + newMember->getJoinDate());
    }
    
    // Read - View all members
    void viewAllMembers() {
        ConsoleUI::printHeader("All Members");
        
        if (members.empty()) {
            ConsoleUI::printWarning("No members found!");
            return;
        }
        
        vector<string> headers = {"ID", "Name", "Email", "Join Date", "Subscription"};
        vector<int> widths = {8, 20, 25, 12, 15};
        
        ConsoleUI::printTableHeader(headers, widths);
        
        for (const Member* member : members) {
            vector<string> row = {
                to_string(member->getId()),
                member->getName(),
                member->getEmail(),
                member->getJoinDate(),
                to_string(member->getSubscriptionId())
            };
            ConsoleUI::printTableRow(row, widths);
        }
    }
    
    // Update - Update member information
    void updateMember() {
        ConsoleUI::printHeader("Update Member");
        
        if (members.empty()) {
            ConsoleUI::printWarning("No members to update!");
            return;
        }
        
        int id = ConsoleUI::getIntInput("Enter member ID to update: ");
        
        Member* member = findMemberById(id);
        if (member == nullptr) {
            ConsoleUI::printError("Member not found!");
            return;
        }
        
        ConsoleUI::printInfo("Current member: " + member->getName());
        ConsoleUI::printInfo("What would you like to update?");
        cout << "  1. Subscription ID" << endl;
        cout << "  0. Cancel" << endl;
        
        int choice = ConsoleUI::getChoice();
        
        switch (choice) {
            case 1: {
                int subId = ConsoleUI::getIntInput("Enter new subscription ID: ");
                member->setSubscriptionId(subId);
                saveMembersToDB();  // Save change
                ConsoleUI::printSuccess("Subscription updated!");
                break;
            }
            case 0:
                ConsoleUI::printInfo("Update cancelled");
                break;
            default:
                ConsoleUI::printError("Invalid choice!");
                break;
        }
    }
    
    // Delete - Remove a member
    void deleteMember() {
        ConsoleUI::printHeader("Delete Member");
        
        if (members.empty()) {
            ConsoleUI::printWarning("No members to delete!");
            return;
        }
        
        int id = ConsoleUI::getIntInput("Enter member ID to delete: ");
        
        for (auto it = members.begin(); it != members.end(); ++it) {
            if ((*it)->getId() == id) {
                string name = (*it)->getName();
                delete *it;
                members.erase(it);
                saveMembersToDB();  // Save change
                ConsoleUI::printSuccess("Member '" + name + "' deleted successfully!");
                return;
            }
        }
        
        ConsoleUI::printError("Member not found!");
    }
    
    // Helper - Find member by ID
    Member* findMemberById(int id) {
        for (Member* member : members) {
            if (member->getId() == id) {
                return member;
            }
        }
        return nullptr;
    }
    
    // ==================== TRAINER CRUD ====================
    
    // Create - Add new trainer
    void addTrainer() {
        ConsoleUI::printHeader("Add New Trainer");
        
        string name = ConsoleUI::getInput("Enter trainer name: ");
        string email = ConsoleUI::getInput("Enter trainer email: ");
        string password = ConsoleUI::getInput("Enter trainer password: ");
        string specialty = ConsoleUI::getInput("Enter trainer specialty: ");
        
        Trainer* newTrainer = new Trainer(name, email, password, specialty);
        trainers.push_back(newTrainer);
        
        // Save to database
        saveTrainersToDB();
        
        ConsoleUI::printSuccess("Trainer added successfully!");
        ConsoleUI::printInfo("Trainer ID: " + to_string(newTrainer->getId()));
    }
    
    // Read - View all trainers
    void viewAllTrainers() {
        ConsoleUI::printHeader("All Trainers");
        
        if (trainers.empty()) {
            ConsoleUI::printWarning("No trainers found!");
            return;
        }
        
        vector<string> headers = {"ID", "Name", "Email", "Specialty", "Assigned Members"};
        vector<int> widths = {8, 20, 25, 20, 18};
        
        ConsoleUI::printTableHeader(headers, widths);
        
        for (const Trainer* trainer : trainers) {
            vector<string> row = {
                to_string(trainer->getId()),
                trainer->getName(),
                trainer->getEmail(),
                trainer->getTrainerSpecialty(),
                to_string(trainer->getAssignedMembers().size())
            };
            ConsoleUI::printTableRow(row, widths);
        }
    }
    
    // Update - Update trainer information
    void updateTrainer() {
        ConsoleUI::printHeader("Update Trainer");
        
        if (trainers.empty()) {
            ConsoleUI::printWarning("No trainers to update!");
            return;
        }
        
        int id = ConsoleUI::getIntInput("Enter trainer ID to update: ");
        
        Trainer* trainer = findTrainerById(id);
        if (trainer == nullptr) {
            ConsoleUI::printError("Trainer not found!");
            return;
        }
        
        ConsoleUI::printInfo("Current trainer: " + trainer->getName());
        ConsoleUI::printInfo("What would you like to update?");
        cout << "  1. Specialty" << endl;
        cout << "  2. Assign Member" << endl;
        cout << "  0. Cancel" << endl;
        
        int choice = ConsoleUI::getChoice();
        
        switch (choice) {
            case 1: {
                string specialty = ConsoleUI::getInput("Enter new specialty: ");
                trainer->setTrainerSpecialty(specialty);
                saveTrainersToDB();  // Save change
                ConsoleUI::printSuccess("Specialty updated!");
                break;
            }
            case 2: {
                if (members.empty()) {
                    ConsoleUI::printWarning("No members available to assign!");
                    break;
                }
                
                viewAllMembers();
                int memberId = ConsoleUI::getIntInput("Enter member ID to assign: ");
                Member* member = findMemberById(memberId);
                
                if (member != nullptr) {
                    trainer->assignMember(member);
                    // Note: Member assignments are not persisted to file
                    ConsoleUI::printSuccess("Member assigned!");
                } else {
                    ConsoleUI::printError("Member not found!");
                }
                break;
            }
            case 0:
                ConsoleUI::printInfo("Update cancelled");
                break;
            default:
                ConsoleUI::printError("Invalid choice!");
                break;
        }
    }
    
    // Delete - Remove a trainer
    void deleteTrainer() {
        ConsoleUI::printHeader("Delete Trainer");
        
        if (trainers.empty()) {
            ConsoleUI::printWarning("No trainers to delete!");
            return;
        }
        
        int id = ConsoleUI::getIntInput("Enter trainer ID to delete: ");
        
        for (auto it = trainers.begin(); it != trainers.end(); ++it) {
            if ((*it)->getId() == id) {
                string name = (*it)->getName();
                delete *it;
                trainers.erase(it);
                saveTrainersToDB();  // Save change
                ConsoleUI::printSuccess("Trainer '" + name + "' deleted successfully!");
                return;
            }
        }
        
        ConsoleUI::printError("Trainer not found!");
    }
    
    // Helper - Find trainer by ID
    Trainer* findTrainerById(int id) {
        for (Trainer* trainer : trainers) {
            if (trainer->getId() == id) {
                return trainer;
            }
        }
        return nullptr;
    }
    
    // ==================== MENU SYSTEM ====================

    // Show main menu (after login)
    void showMainMenu() {
        if (!isLoggedIn()) {
            ConsoleUI::printError("Please login first!");
            return;
        }

        vector<string> options = {
            "Manage Members",
            "Manage Trainers",
            "Generate Report",
            "Manage Account",
            "Logout"
        };
        
        ConsoleUI::printMenu("El-Forma Gym Management - Main Menu", options);
    }
    
    // Show members management menu
    void showMembersMenu() {
        vector<string> options = {
            "Add Member",
            "View All Members",
            "Update Member",
            "Delete Member"
        };
        
        ConsoleUI::printMenu("Members Management", options);
    }
    
    // Show trainers management menu
    void showTrainersMenu() {
        vector<string> options = {
            "Add Trainer",
            "View All Trainers",
            "Update Trainer",
            "Delete Trainer"
        };
        
        ConsoleUI::printMenu("Trainers Management", options);
    }
    
    // Handle members menu
    void handleMembersMenu() {
        while (true) {
            showMembersMenu();
            int choice = ConsoleUI::getChoice();
            
            switch (choice) {
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
    void handleTrainersMenu() {
        while (true) {
            showTrainersMenu();
            int choice = ConsoleUI::getChoice();
            
            switch (choice) {
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
                case 3: // Update
                    updateTrainer();
                    ConsoleUI::pause();
                    break;
                case 4: // Delete
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
                    case 1: // Manage Members
                        handleMembersMenu();
                        break;
                    case 2: // Manage Trainers
                        handleTrainersMenu();
                        break;
                    case 3: // Generate Report
                        currentAdmin->generateReport();
                        ConsoleUI::pause();
                        break;
                    case 4: // Manage Account
                        currentAdmin->manageAccount(currentAdmin->getId());
                        ConsoleUI::pause();
                        break;
                    case 5: // Logout
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
