#ifndef MEMBER_SERVICE_H
#define MEMBER_SERVICE_H

#include <iostream>
#include <vector>
#include <string>

#include "../entities/Member.h"
#include "../services/ConsoleUI.h"
#include "../services/TrainerService.h"

using namespace std;

// MemberService class - handles member operations with UI
class MemberService
{
private:
    // Vector to store members in memory (shared across all instances)
    static vector<Member *> members;
    static bool initialized;

    // Initialize with fake members for testing
    void initialize()
    {
        if (!initialized)
        {
            // Create fake members using add method (IDs auto-assigned)
            Member *m1 = new Member("Mohamed", "mohamed@gmail.com", "123", "2024-01-15");
            m1->setSubscriptionId(1);
            members.push_back(m1);

            Member *m2 = new Member("Ahmed", "ahmed@gmail.com", "123", "2024-02-20");
            m2->setSubscriptionId(2);
            members.push_back(m2);

            Member *m3 = new Member("Mostafa", "mostafa@gmail.com", "123", "2024-03-10");
            m3->setSubscriptionId(1);
            members.push_back(m3);

            initialized = true;
        }
    }

    // Add or Update Member Helper - Normalize subscription type
    string getNormalizedSubscriptionType(const string &type)
    {
        string lowerType = ConsoleUI::toLower(type);
        if (lowerType == "standard" || lowerType == "std" || lowerType == "s" || lowerType == "1")
        {
            return "Standard";
        }
        else if (lowerType == "premium" || lowerType == "prem" || lowerType == "p" || lowerType == "2")
        {
            return "Premium";
        }
        return "Unknown";
    }

public:
    // Constructor
    MemberService()
    {
        initialize();
    }

    // Add new member with UI
    void addMember()
    {
        // Draw the New Form UI
        vector<string> data = ConsoleUI::getFormData("REGISTER NEW MEMBER",
                                                     {"Name", "Email", "Password", "Type (Standard [s, std, 1]/Premium [p, prem, 2])"});

        if (data.empty())
            return; // ESC Pressed: Cancelled

        // Validate subscription type
        string typeInput = getNormalizedSubscriptionType(data[3]);

        if (typeInput == "Unknown")
        {
            ConsoleUI::printError("Invalid Subscription Type!");
            ConsoleUI::printInfo("Allowed: Standard (s, std, 1), Premium (p, prem, 2)");
            ConsoleUI::pause();
            return; // Cancel the operation, don't save
        }

        Member *newMember = new Member(data[0], data[1], data[2]);

        // Set the ID logic
        if (typeInput == "Standard")
            newMember->setSubscriptionId(1);
        else
            newMember->setSubscriptionId(2);

        // Store
        members.push_back(newMember);

        ConsoleUI::printSuccess("Member added successfully!");
        ConsoleUI::printInfo("Member ID: " + to_string(newMember->getId()));
        ConsoleUI::printInfo("Type: " + typeInput); // Show them what we saved
        ConsoleUI::pause();
    }

    // View all members with UI
    void viewAllMembers()
    {
        ConsoleUI::printHeader("All Members");

        if (members.empty())
        {
            ConsoleUI::printWarning("No members found!");
            return;
        }

        vector<string> headers = {"ID", "Name", "Email", "Join Date", "Subscription"};
        vector<int> widths = {8, 20, 25, 12, 15};

        ConsoleUI::printTableHeader(headers, widths);

        for (const Member *member : members)
        {
            vector<string> row = {
                to_string(member->getId()),
                member->getName(),
                member->getEmail(),
                member->getJoinDate(),
                to_string(member->getSubscriptionId())};
            ConsoleUI::printTableRow(row, widths);
        }
        ConsoleUI::pause();
    }

    // Update member with UI
    void updateMember()
    {
        if (members.empty())
        {
            ConsoleUI::printWarning("No members to update!");
            return;
        }

        // Use simple input for ID
        int id = ConsoleUI::getIntInput("\nEnter member ID to update: ");

        Member *member = findMemberById(id);

        if (member == nullptr)
        {
            ConsoleUI::printError("Member not found!");
            return;
        }

        // Use the New Arrow Menu UI
        vector<string> opts = {"Update Subscription Type", "Cancel"};
        int choice = ConsoleUI::getMenuSelection("UPDATE MEMBER: " + member->getName(), opts);

        if (choice == 0)
        {
            int subId = 0;
            string inputSubscriptionType = ConsoleUI::getInput("Enter new subscription type: ");

            // Validate subscription type
            string subscriptionType = getNormalizedSubscriptionType(inputSubscriptionType);

            if (subscriptionType == "Unknown")
            {
                ConsoleUI::printError("Invalid Subscription Type!");
                ConsoleUI::printInfo("Allowed: Standard (s, std, 1), Premium (p, prem, 2)");
                ConsoleUI::pause();
                return; // Cancel the operation, don't save
            }

            // Set the ID logic
            if (subscriptionType == "Standard")
                member->setSubscriptionId(1);
            else
                member->setSubscriptionId(2);

            ConsoleUI::printSuccess("Subscription updated!");
        }
        else
        {
            ConsoleUI::printInfo("Update cancelled");
        }

        ConsoleUI::pause();
    }

    // Delete member with UI
    void deleteMember()
    {
        ConsoleUI::printHeader("Delete Member");

        if (members.empty())
        {
            ConsoleUI::printWarning("No members to delete!");
            ConsoleUI::pause();
            return;
        }

        int id = ConsoleUI::getIntInput("Enter member ID to delete: ");

        for (auto it = members.begin(); it != members.end(); ++it)
        {
            if ((*it)->getId() == id)
            {
                string name = (*it)->getName();

                // --- CASCADING DELETE ---
                // Before deleting the member from memory, remove them from any Trainers.
                TrainerService::removeMemberFromAllTrainers(id);

                delete *it;
                members.erase(it);
                ConsoleUI::printSuccess("Member '" + name + "' deleted successfully!");
                ConsoleUI::pause();
                return;
            }
        }

        ConsoleUI::printError("Member not found!");
        ConsoleUI::pause();
    }

    // Find member by ID (internal use)
    Member *findMemberById(int id)
    {
        for (Member *member : members)
        {
            if (member->getId() == id)
            {
                return member;
            }
        }
        return nullptr;
    }

    // Check if members exist
    bool isEmpty()
    {
        return members.empty();
    }

    // Get all members (for trainer assignment)
    vector<Member *> getAllMembers()
    {
        return members;
    }
};

// Initialize static members
vector<Member *> MemberService::members;
bool MemberService::initialized = false;

#endif
