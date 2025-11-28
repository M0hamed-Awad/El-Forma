#ifndef ID_MANAGER_H
#define ID_MANAGER_H

#include <iostream>
#include <string>
#include <map>

using namespace std;

// IDManager class - manages unique IDs for different entity types in memory
class IDManager {
private:
    // Static map to store last ID for each entity type (in-memory)
    static map<string, int> idMap;

public:
    // Get last ID for entity type
    static int getLastID(const string& entityType) {
        if (idMap.find(entityType) == idMap.end()) {
            return 0; // Not found, return 0
        }
        return idMap[entityType];
    }
    
    // Save last ID for entity type
    static bool saveLastID(const string& entityType, int lastID) {
        idMap[entityType] = lastID;
        return true;
    }
    
    // Get next available ID (last + 1)
    static int getNextID(const string& entityType) {
        return getLastID(entityType) + 1;
    }
};

// Initialize static member
map<string, int> IDManager::idMap;

#endif // ID_MANAGER_H
