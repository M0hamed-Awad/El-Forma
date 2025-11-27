#ifndef DB_H
#define DB_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>


using namespace std;

// Generic Database class for file operations
class DB {
protected:
    string filename;

public:
    // Constructor
    DB(const string& file) : filename(file) {}

    // Check if file exists
    bool fileExists() const {
        ifstream file(filename);
        return file.good();
    }

    // Read all lines from file
    vector<string> readLines() const {
        vector<string> lines;
        ifstream file(filename);
        
        if (!file.is_open()) {
            cerr << "[DB] Warning: Could not open " << filename << endl;
            return lines;
        }

        string line;
        while (getline(file, line)) {
            if (!line.empty()) {  // Skip empty lines
                lines.push_back(line);
            }
        }

        file.close();
        return lines;
    }

    // Write lines to file (overwrite)
    bool writeLines(const vector<string>& lines) const {
        ofstream file(filename);
        
        if (!file.is_open()) {
            cerr << "[DB] Error: Could not open " << filename << " for writing" << endl;
            return false;
        }

        for (const string& line : lines) {
            file << line << endl;
        }

        file.close();
        return true;
    }

    // Append a line to file
    bool appendLine(const string& line) const {
        ofstream file(filename, ios::app);
        
        if (!file.is_open()) {
            cerr << "[DB] Error: Could not open " << filename << " for appending" << endl;
            return false;
        }

        file << line << endl;
        file.close();
        return true;
    }

    // Clear file content
    bool clearFile() const {
        ofstream file(filename, ios::trunc);
        
        if (!file.is_open()) {
            cerr << "[DB] Error: Could not open " << filename << " for clearing" << endl;
            return false;
        }

        file.close();
        return true;
    }

    // Get filename
    string getFilename() const {
        return filename;
    }

    // Helper: Split string by delimiter
    static vector<string> split(const string& str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }

    // Helper: Trim whitespace from string
    static string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }
};

#endif // DB_H
