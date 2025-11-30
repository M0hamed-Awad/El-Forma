#ifndef MEMBER_H
#define MEMBER_H

#include "User.h"
#include <ctime>
#include <string>

class Member : public User
{
    string joinDate;
    int subscriptionId; // 1 = Standard, 2 = Premium
    
    inline static int nextMemberId = 0;
    inline static bool loadingFromDB = false;

    // TODO [V2.0]: Implement Loyalty Points System
    // int accessPoints; 
    // vector<Transaction> paymentHistory;

    string getCurrentDate()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);

        string year = to_string(1900 + ltm->tm_year);
        string month = to_string(1 + ltm->tm_mon);
        string day = to_string(ltm->tm_mday);

        // Add leading '0' for months/days -----> "5" -> "05"
        if (month.length() == 1)
            month = "0" + month;
        if (day.length() == 1)
            day = "0" + day;

        return year + "-" + month + "-" + day;
    }

public:
    // Automatic Join Date Assignment
    Member(string memberName, string memberEmail, string memberPassword)
        : User(memberName, memberEmail, memberPassword)
    {
        joinDate = getCurrentDate();
        subscriptionId = 0;
        
        if (!loadingFromDB) {
            nextMemberId++;
            id = nextMemberId;
        }
    }

    // Manual Join Date Assignment (For Backdating)
    Member(string memberName, string memberEmail, string memberPassword, string specificDate)
        : User(memberName, memberEmail, memberPassword)
    {
        joinDate = specificDate;
        subscriptionId = 0;
        
        if (!loadingFromDB) {
            nextMemberId++;
            id = nextMemberId;
        }
    }

    // Getters
    int getSubscriptionId() const {return subscriptionId;}
    string getJoinDate() const { return joinDate;}
    
    // Static ID management
    static void setNextMemberId(int lastId) { nextMemberId = lastId; }
    static void setLoadingMode(bool loading) { loadingFromDB = loading; }

    // Setters
    void setSubscriptionId(int subscId) { subscriptionId = subscId; }
};

#endif