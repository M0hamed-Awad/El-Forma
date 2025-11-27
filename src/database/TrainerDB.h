#ifndef TRAINER_DB_H
#define TRAINER_DB_H

#include <iostream>
#include <vector>
#include <string>

#include "../services/DB.h"
#include "../entities/Trainer.h"
#include "IDManager.h"

using namespace std;

// TrainerDB class - handles trainer-specific database operations
class TrainerDB {
private:
    DB db;

public:
    // Constructor
    TrainerDB(const string& filename = "trainers.txt") : db(filename) {}

    // Load all trainers from file
    // Format: id,name,email,password,specialty
    vector<Trainer*> loadTrainers() const {
        vector<Trainer*> trainers;
        vector<string> lines = db.readLines();
        int maxId = 0;

        for (const string& line : lines) {
            vector<string> fields = DB::split(line, ',');
            
            // Ensure we have exactly 5 fields: id, name, email, password, specialty
            if (fields.size() >= 5) {
                int id = 0;
                string name = DB::trim(fields[1]);
                string email = DB::trim(fields[2]);
                string password = DB::trim(fields[3]);
                string specialty = DB::trim(fields[4]);
                
                // Parse ID
                try {
                    id = stoi(DB::trim(fields[0]));
                    if (id > maxId) maxId = id;
                } catch (...) {
                    continue; // Skip invalid ID
                }
                
                if (!name.empty() && !email.empty() && !password.empty() && !specialty.empty()) {
                    Trainer* trainer = new Trainer(name, email, password, specialty);
                    trainer->setId(id);  // Restore ID
                    trainers.push_back(trainer);
                }
            }
        }
        
        // Update the ID counter
        if (maxId > 0) {
            IDManager::saveLastID("Trainer", maxId);
        }

        return trainers;
    }

    // Save all trainers to file
    bool saveTrainers(const vector<Trainer*>& trainers) const {
        vector<string> lines;
        int maxId = 0;
        
        for (const Trainer* trainer : trainers) {
            // Format: id,name,email,password,specialty
            string line = to_string(trainer->getId()) + "," +
                         trainer->getName() + "," + 
                         trainer->getEmail() + "," +
                         "******" + "," +  // Don't save actual password
                         trainer->getTrainerSpecialty();
            lines.push_back(line);
            
            if (trainer->getId() > maxId) {
                maxId = trainer->getId();
            }
        }
        
        // Update ID counter
        if (maxId > 0) {
            IDManager::saveLastID("Trainer", maxId);
        }

        return db.writeLines(lines);
    }

    // Add a new trainer
    bool addTrainer(const Trainer* trainer) const {
        string line = trainer->getName() + "," + 
                     trainer->getEmail() + "," +
                     "******" + "," +  // Password placeholder
                     trainer->getTrainerSpecialty();
        return db.appendLine(line);
    }

    // Update a trainer (replace entire file)
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
        vector<Trainer*> trainers = loadTrainers();
        
        for (Trainer* trainer : trainers) {
            if (trainer->getId() == id) {
                // Clean up other trainers
                for (Trainer* t : trainers) {
                    if (t != trainer) {
                        delete t;
                    }
                }
                return trainer;
            }
        }
        
        // Clean up all trainers if not found
        for (Trainer* t : trainers) {
            delete t;
        }
        return nullptr;
    }

    // Find trainer by email
    Trainer* findTrainerByEmail(const string& email) const {
        vector<Trainer*> trainers = loadTrainers();
        
        for (Trainer* trainer : trainers) {
            if (trainer->getEmail() == email) {
                // Clean up other trainers
                for (Trainer* t : trainers) {
                    if (t != trainer) {
                        delete t;
                    }
                }
                return trainer;
            }
        }
        
        // Clean up all trainers if not found
        for (Trainer* t : trainers) {
            delete t;
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

#endif // TRAINER_DB_H
