#ifndef TRAINER_SERVICE_H
#define TRAINER_SERVICE_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Trainer.h"
#include "../entities/Member.h"
#include "../services/ConsoleUI.h"

using namespace std;

// TrainerService class - handles trainer operations with UI
class TrainerService {
private:
    // Vector to store trainers in memory (shared across all instances)
    static vector<Trainer*> trainers;
    static bool initialized;
    
    // Initialize with fake trainers for testing
    void initialize() {
        if (!initialized) {
            // Create fake trainers
            trainers.push_back(new Trainer("Amir", "amir@gmail.com", "trainer123", "Cardio"));
            trainers.push_back(new Trainer("Kareem", "kareem@gmail.com", "trainer456", "Strength Training"));
            trainers.push_back(new Trainer("Maged", "maged@gmail.com", "trainer789", "Yoga"));
            
            initialized = true;
        }
    }

public:
    // Constructor
    TrainerService() {
        initialize();
    }

    // Add new trainer with UI
    void addTrainer() {
        // Use the New Form UI
        vector<string> data = ConsoleUI::getFormData("ADD NEW TRAINER", 
            {"Name", "Email", "Password", "Specialty"});

        if (data.empty()) return; // Cancelled
                
        Trainer* newTrainer = new Trainer(data[0], data[1], data[2], data[3]);
        trainers.push_back(newTrainer);
        
        ConsoleUI::printSuccess("Trainer added successfully!");
        ConsoleUI::printInfo("Trainer ID: " + to_string(newTrainer->getId()));
        ConsoleUI::pause();
    }
    
    // View all trainers with UI
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
        ConsoleUI::pause();
    }
    
    // View assigned members for a trainer with UI
    void viewAssignedMembers() {
        ConsoleUI::printHeader("Assigned Members");
        
        if (trainers.empty()) {
            ConsoleUI::printWarning("No trainers found!");
            return;
        }
        
        int id = ConsoleUI::getIntInput("Enter trainer ID to view assigned members: ");
        
        Trainer* trainer = findTrainerById(id);
        if (trainer == nullptr) {
            ConsoleUI::printError("Trainer not found!");
            return;
        }
        
        vector<string> headers = {"ID", "Name", "Email", "Specialty"};
        vector<int> widths = {8, 20, 25, 20};
        
        ConsoleUI::printTableHeader(headers, widths);
        
        for (const Member* member : trainer->getAssignedMembers()) {
            vector<string> row = {
                to_string(member->getId()),
                member->getName(),
                member->getEmail(),
                trainer->getTrainerSpecialty()
            };
            ConsoleUI::printTableRow(row, widths);
        }
        ConsoleUI::pause();
    }
    
    // Update trainer with UI
    void updateTrainer(vector<Member*> availableMembers) {
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
        
        //  Use the new Arrow Menu for choices
        vector<string> opts = {
            "Update Specialty", 
            "Assign Member", 
            "Cancel"
        };
        
        int choice = ConsoleUI::getMenuSelection("UPDATE TRAINER: " + trainer->getName(), opts);
        
        // Handle selection by Index
        if (choice == 0) { // Update Specialty
            string specialty = ConsoleUI::getInput("Enter new specialty: ");
            trainer->setTrainerSpecialty(specialty);
            ConsoleUI::printSuccess("Specialty updated!");
        } 
        else if (choice == 1) { // Assign Member
            if (availableMembers.empty()) {
                ConsoleUI::printWarning("No members available to assign!");
                return;
            }
            
            // Display available members table
            ConsoleUI::printHeader("Available Members");
            vector<string> headers = {"ID", "Name", "Email"};
            vector<int> widths = {8, 20, 25};
            ConsoleUI::printTableHeader(headers, widths);
            
            for (const Member* member : availableMembers) {
                vector<string> row = {
                    to_string(member->getId()),
                    member->getName(),
                    member->getEmail()
                };
                ConsoleUI::printTableRow(row, widths);
            }
            
            // Select member
            int memberId = ConsoleUI::getIntInput("Enter member ID to assign: ");
            
            Member* memberToAssign = nullptr;
            for (Member* m : availableMembers) {
                if (m->getId() == memberId) {
                    memberToAssign = m;
                    break;
                }
            }
            
            if (memberToAssign != nullptr) {
                trainer->assignMember(memberToAssign);
                // Note: Success message is handled inside trainer->assignMember
                // But we can add a pause here if needed
            } else {
                ConsoleUI::printError("Member not found!");
            }
        } 
        else { // Cancel (Choice 2 or ESC)
            ConsoleUI::printInfo("Update cancelled");
        }
    }
    
    // Delete trainer with UI
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
                ConsoleUI::printSuccess("Trainer '" + name + "' deleted successfully!");
                return;
            }
        }
        
        ConsoleUI::printError("Trainer not found!");
    }
    
    // Find trainer by ID (internal use)
    Trainer* findTrainerById(int id) {
        for (Trainer* trainer : trainers) {
            if (trainer->getId() == id) {
                return trainer;
            }
        }
        return nullptr;
    }
    
    // Check if trainers exist
    bool isEmpty() {
        return trainers.empty();
    }
};

// Initialize static members
vector<Trainer*> TrainerService::trainers;
bool TrainerService::initialized = false;

#endif // TRAINER_SERVICE_H
