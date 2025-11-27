#include "ConsoleUI.h"
#include <vector>

using namespace std;

int main() {

    // Display login menu
    ConsoleUI::printMenu("GYM MANAGEMENT SYSTEM - LOGIN", {
        "Admin Login",
        "View Information",
        });

    int choice = ConsoleUI::getChoice();

    // Display success/error
    ConsoleUI::printSuccess("Login successful!");
    ConsoleUI::printError("Invalid credentials!");

    // Display table
    vector<string> headers = { "ID", "Name", "Membership" };
    vector<int> widths = { 10, 30, 20 };
    ConsoleUI::printTableHeader(headers, widths);
    ConsoleUI::printTableRow({ "1", "John Doe", "Premium" }, widths);

    return 0;
}