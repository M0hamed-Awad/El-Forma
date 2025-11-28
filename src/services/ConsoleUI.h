#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

// To prevent Windows Header conflicts
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

// Windows
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
// Not Windows
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

class ConsoleUI
{
private:
    static const int DEFAULT_WIDTH = 80;

    // --- NAVIGATION KEYS ---
    static const int KEY_UP = 72;
    static const int KEY_DOWN = 80;
    static const int KEY_LEFT = 75;
    static const int KEY_RIGHT = 77;
    static const int KEY_HOME = 71;
    static const int KEY_END = 79;

    // --- ACTION KEYS ---
    static const int ENTER_KEY = 13;
    static const int BACKSPACE_KEY = 8;
    static const int ESC_KEY = 27;

public:
    // Clear screen
    static void clear()
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }

    // Wait for user to press any key
    static void pause()
    {
        cout << "\nPress any key to continue...";
        _getch();
    }

    // Cursor Mover
    static void moveCursor(int row, int col)
    {
        cout << "\033[" << row << ";" << col << "H";
    }

    // --- SIMPLE INPUT HELPERS ---
    // Get user input with prompt
    static string getInput(string prompt)
    {
        string input;
        cout << "\n"
             << prompt;      // Show the question
        getline(cin, input); // Get the WHOLE line
        return input;        // Send it back
    }

    // Get integer input with validation
    static int getIntInput(string prompt)
    {
        int input;
        while (true)
        {
            cout << "\n"
                 << prompt; // Show the question
            // Try to read a number
            if (cin >> input)
            {
                cin.ignore(10000, '\n'); // Clear the input buffer
                return input;
            }
            else
            {
                cout << "\033[31m[ERROR] Invalid number.\033[0m";
                cin.clear();
                cin.ignore(10000, '\n'); // Throw away the bad text
            }
        }
    }

    // --- OUTPUT HELPERS ---
    // Print header with title
    static void printHeader(string title)
    {
        clear();
        cout << "=== " << title << " ===" << endl;
    }

    // Print success message
    static void printSuccess(string msg)
    {
        cout << "\n\033[32m[SUCCESS] " << msg << "\033[0m" << endl;
    }

    // Print error message
    static void printError(string msg)
    {
        cout << "\n\033[31m[ERROR] " << msg << "\033[0m" << endl;
    }

    // Print warning message
    static void printWarning(string msg)
    {
        cout << "\n\033[33m[WARNING] " << msg << "\033[0m" << endl;
    }

    // Print info message
    static void printInfo(string msg)
    {
        cout << "\n[INFO] " << msg << endl;
    }

    // Print horizontal line
    static void printLine(char ch = '=', int width = 0)
    {
        if (width == 0)
            width = getConsoleWidth();
        std::cout << std::string(width, ch) << std::endl;
    }

    // Print centered text
    static void printCentered(const std::string &text, int width = 0)
    {
        if (width == 0)
            width = getConsoleWidth();
        int padding = (width - text.length()) / 2;
        if (padding > 0)
        {
            std::cout << std::string(padding, ' ') << text << std::endl;
        }
        else
        {
            std::cout << text << std::endl;
        }
    }

    // Draw Menu Title
    static void drawMenuTitle(const string &title)
    {
        cout << "\n=== " << title << " ===\n";
        cout << "----------------------------------------\n";
    }

    // Draw Menu Options
    static void drawMenuOptions(const vector<string> &options, int currentSelection)
    {
        // Color every Option in the Menu
        for (int i = 0; i < options.size(); i++)
        {
            // Selected Option
            if (i == currentSelection)
            {
                // Highlight (CYAN)
                cout << "\033[96m -> " << options[i] << " \033[0m\n";
            }
            // Not Selected Option
            else
            {
                cout << "    " << options[i] << "\n";
            }
        }
        cout << "----------------------------------------\n";
    }

    // Draw the Menu Body
    static void drawMenu(const string &title, const vector<string> &options, int currentSelection)
    {
        clear();
        drawMenuTitle(title);
        drawMenuOptions(options, currentSelection);
    }

    // Handle Arrow UP
    static void moveUp(int &currentSelection, const vector<string> &options)
    {
        currentSelection--; // Updating the Index Value
        if (currentSelection < 0) // For Circular Functionality
            currentSelection = options.size() - 1;
    }

    // Handle Arrow DOWN
    static void moveDown(int &currentSelection, const vector<string> &options)
    {
        currentSelection++; // Updating the Index Value
        if (currentSelection >= options.size()) // For Circular Functionality
            currentSelection = 0;
    }

    // Handle Arrow Keys
    static void handleArrowKey(int &currentSelection, const vector<string> &options)
    {
        char key = _getch();

        // Detect the UP arrow
        if (key == KEY_UP)
            moveUp(currentSelection, options);
        // Detect the DOWN arrow
        else if (key == KEY_DOWN)
            moveDown(currentSelection, options);
    }

    static bool handleMenuNavigation(int &currentSelection, const vector<string> &options)
    {
        char key = _getch();

        if (key == -32 || key == 224) // Arrow keys
        {
            handleArrowKey(currentSelection, options);
            return false;
        }
        // Detect ENTER Key
        else if (key == ENTER_KEY)
        {
            return true; // User confirmed selection
        }

        return false;
    }

    // --- DRAWING THE MENU ---
    // Returns index (0, 1, 2...)
    static int getMenuSelection(string title, vector<string> options)
    {
        int currentSelection = 0;

        while (true)
        {
            drawMenu(title, options, currentSelection);

            bool isEnterPressed = handleMenuNavigation(currentSelection, options);
            if (isEnterPressed)
                return currentSelection;
        }
    }

    // Print menu with options
    static void printMenu(const std::string &title, const std::vector<std::string> &options)
    {
        printHeader(title);
        for (size_t i = 0; i < options.size(); i++)
        {
            std::cout << "  " << (i + 1) << ". " << options[i] << std::endl;
        }
        std::cout << "  0. Exit/Back" << std::endl;
        printLine('-');
    }

    // Print sub-header
    static void printSubHeader(const std::string &text)
    {
        std::cout << std::endl;
        printLine('-');
        std::cout << "  " << text << std::endl;
        printLine('-');
    }

    // Get double input with validation
    static double getDoubleInput(const std::string &prompt)
    {
        std::string input;
        double value;
        while (true)
        {
            std::cout << prompt;
            std::getline(std::cin, input);
            std::stringstream ss(input);
            if (ss >> value && ss.eof())
            {
                return value;
            }
            printError("Invalid input. Please enter a valid number.");
        }
    }

    // Print table header
    static void printTableHeader(const std::vector<std::string> &headers,
                                 const std::vector<int> &widths)
    {
        std::cout << std::endl;
        for (size_t i = 0; i < headers.size(); i++)
        {
            std::cout << std::left << std::setw(widths[i]) << headers[i] << " ";
        }
        std::cout << std::endl;

        int totalWidth = 0;
        for (int w : widths)
            totalWidth += w + 1;
        printLine('-', totalWidth);
    }

    // Print table row
    static void printTableRow(const std::vector<std::string> &columns,
                              const std::vector<int> &widths)
    {
        for (size_t i = 0; i < columns.size() && i < widths.size(); i++)
        {
            std::string text = columns[i];
            if (text.length() > (size_t)widths[i])
            {
                text = text.substr(0, widths[i] - 3) + "...";
            }
            std::cout << std::left << std::setw(widths[i]) << text << " ";
        }
        std::cout << std::endl;
    }

    // Print a box with content
    static void printBox(const std::string &content, int width = 60)
    {
        printLine('+', width);

        std::istringstream iss(content);
        std::string line;
        while (std::getline(iss, line))
        {
            int padding = width - line.length() - 4;
            std::cout << "| " << line;
            if (padding > 0)
            {
                std::cout << std::string(padding, ' ');
            }
            std::cout << " |" << std::endl;
        }

        printLine('+', width);
    }

    // Print choice prompt
    static int getChoice(const std::string &prompt = "Enter your choice: ")
    {
        return getIntInput(prompt);
    }

    // Confirm action
    static bool confirm(const std::string &message)
    {
        std::string input;
        std::cout << message << " (y/n): ";
        std::getline(std::cin, input);
        return (input == "y" || input == "Y" || input == "yes" || input == "Yes");
    }

    // Print section divider
    static void printDivider()
    {
        std::cout << std::endl;
        printLine('*', 40);
        std::cout << std::endl;
    }
};

#endif // CONSOLE_UI_H