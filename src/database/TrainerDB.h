#ifndef TRAINER_DB_H
#define TRAINER_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Trainer.h"

using namespace std;

// TrainerDB class - handles trainer-specific database operations (in-memory)
class TrainerDB {
private:
    // Vector to store trainers in memory (shared across all instances)
    static vector<Trainer*> trainers;
    static bool initialized;
    
    // Initialize with fake trainers for testing
    void initialize() {
        if (!initialized) {
            // Create fake trainers using add method (IDs auto-assigned)
            addTrainer(new Trainer("Amir", "amir@gmail.com", "trainer123", "Cardio"));
            addTrainer(new Trainer("Kareem", "kareem@gmail.com", "trainer456", "Strength Training"));
            addTrainer(new Trainer("Maged", "maged@gmail.com", "trainer789", "Yoga"));
            
            initialized = true;
        }
    }

public:
    // Constructor
    TrainerDB() {
        initialize();
    }

    // Load all trainers (returns the in-memory vector)
    vector<Trainer*> loadTrainers() {
        initialize();
        return trainers;
    }

    // Save all trainers (updates the in-memory vector)
    bool saveTrainers(const vector<Trainer*>& newTrainers) {
        trainers = newTrainers;
        return true;
    }

    // Add a new trainer
    bool addTrainer(Trainer* trainer) {
        if (!initialized) {
            // During initialization, just add without calling initialize()
            trainers.push_back(trainer);
        } else {
            initialize();
            trainers.push_back(trainer);
        }
        return true;
    }

    // Find trainer by ID
    Trainer* findTrainerById(int id) {
        initialize();
        for (Trainer* trainer : trainers) {
            if (trainer->getId() == id) {
                return trainer;
            }
        }
        return nullptr;
    }

    // Find trainer by email
    Trainer* findTrainerByEmail(const string& email) {
        initialize();
        for (Trainer* trainer : trainers) {
            if (trainer->getEmail() == email) {
                return trainer;
            }
        }
        return nullptr;
    }
};

// Initialize static members
vector<Trainer*> TrainerDB::trainers;
bool TrainerDB::initialized = false;

#endif // TRAINER_DB_H
