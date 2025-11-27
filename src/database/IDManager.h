#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// IDManager class - manages unique IDs for different entity types
class IDManager {
private:
    static const string ID_FILE;
    
    // Helper to read ID for specific entity type
    static int readIDFromFile(const string& entityType) {
        ifstream file(ID_FILE);
        if (!file.is_open()) {
            return 0; // File doesn't exist, start from 0
        }
        
        string line;
        while (getline(file, line)) {
            // Format: entityType:lastID
            size_t pos = line.find(':');
            if (pos != string::npos) {
                string type = line.substr(0, pos);
                if (type == entityType) {
                    try {
                        int id = stoi(line.substr(pos + 1));
                        file.close();
                        return id;
                    } catch (...) {
                        file.close();
                        return 0;
                    }
                }
            }
        }
        
        file.close();
        return 0; // Entity type not found, start from 0
    }
    
    // Helper to write ID for specific entity type
    static bool writeIDToFile(const string& entityType, int lastID) {
        // Read all current IDs
        ifstream inFile(ID_FILE);
        string content;
        bool found = false;
        
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                size_t pos = line.find(':');
                if (pos != string::npos) {
                    string type = line.substr(0, pos);
                    if (type == entityType) {
                        content += entityType + ":" + to_string(lastID) + "\n";
                        found = true;
                    } else {
                        content += line + "\n";
                    }
                } else {
                    content += line + "\n";
                }
            }
            inFile.close();
        }
        
        // If entity type not found, append it
        if (!found) {
            content += entityType + ":" + to_string(lastID) + "\n";
        }
        
        // Write back to file
        ofstream outFile(ID_FILE);
        if (!outFile.is_open()) {
            return false;
        }
        
        outFile << content;
        outFile.close();
        return true;
    }

public:
    // Get last ID for entity type
    static int getLastID(const string& entityType) {
        return readIDFromFile(entityType);
    }
    
    // Save last ID for entity type
    static bool saveLastID(const string& entityType, int lastID) {
        return writeIDToFile(entityType, lastID);
    }
    
    // Get next available ID (last + 1)
    static int getNextID(const string& entityType) {
        return getLastID(entityType) + 1;
    }
};

// Initialize static member
const string IDManager::ID_FILE = "ids.txt";

#endif // ID_MANAGER_H
