#ifndef TRAINER_DB_H
#define TRAINER_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Trainer.h"
#include "IDManager.h"

using namespace std;

// TrainerDB class - handles trainer-specific database operations (in-memory)
class TrainerDB {
private:
    // Static vector to store trainers in memory
    static vector<Trainer*> trainers;
    static bool initialized;
    
    // Initialize with fake trainers for testing
    static void initialize() {
        if (!initialized) {
            // Create fake trainers for testing
            Trainer* trainer1 = new Trainer("Alex Martinez", "alex@gmail.com", "trainer123", "Cardio");
            trainer1->setId(1);
            trainers.push_back(trainer1);
            
            Trainer* trainer2 = new Trainer("Emma Brown", "emma@gmail.com", "trainer456", "Strength Training");
            trainer2->setId(2);
            trainers.push_back(trainer2);
            
            Trainer* trainer3 = new Trainer("Chris Lee", "chris@gmail.com", "trainer789", "Yoga");
            trainer3->setId(3);
            trainers.push_back(trainer3);
            
            // Update ID counter
            IDManager::saveLastID("Trainer", 3);
            initialized = true;
        }
    }

public:
    // Constructor
    TrainerDB() {
        initialize();
    }

    // Load all trainers (returns the in-memory vector)
    vector<Trainer*> loadTrainers() const {
        initialize();
        return trainers;
    }

    // Save all trainers (updates the in-memory vector)
    bool saveTrainers(const vector<Trainer*>& newTrainers) const {
        trainers = newTrainers;
        
        // Update ID counter based on trainers
        int maxId = 0;
        for (const Trainer* trainer : trainers) {
            if (trainer->getId() > maxId) {
                maxId = trainer->getId();
            }
        }
        if (maxId > 0) {
            IDManager::saveLastID("Trainer", maxId);
        }
        
        return true;
    }

    // Add a new trainer
    bool addTrainer(Trainer* trainer) const {
        trainers.push_back(trainer);
        
        // Update ID counter
        if (trainer->getId() > IDManager::getLastID("Trainer")) {
            IDManager::saveLastID("Trainer", trainer->getId());
        }
        
        return true;
    }

    // Update a trainer (replace entire vector)
    bool updateTrainer(const vector<Trainer*>& allTrainers) const {
        return saveTrainers(allTrainers);
    }

    // Delete a trainer (save all except the one to delete)
    bool deleteTrainer(int trainerId, const vector<Trainer*>& allTrainers) const {
        vector<Trainer*> remainingTrainers;
        
        for (Trainer* trainer : allTrainers) {
            if (trainer->getId() != trainerId) {
                remainingTrainers.push_back(trainer);
            }
        }
        
        return saveTrainers(remainingTrainers);
    }

    // Find trainer by ID
    Trainer* findTrainerById(int id) const {
        for (Trainer* trainer : trainers) {
            if (trainer->getId() == id) {
                return trainer;
            }
        }
        
        return nullptr;
    }

    // Find trainer by email
    Trainer* findTrainerByEmail(const string& email) const {
        for (Trainer* trainer : trainers) {
            if (trainer->getEmail() == email) {
                return trainer;
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
vector<Trainer*> TrainerDB::trainers;
bool TrainerDB::initialized = false;

#endif // TRAINER_DB_H
