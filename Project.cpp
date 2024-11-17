#include <iostream>
#include <unordered_map>
#include "Display.h"
#include "Class.h"
#include "validation.h"

using namespace std;

// Global storage for students and admins
unordered_map<string, Student> students;
unordered_map<string, Admin> admins;

// Main function
int main() {
    Validation validation;
    // Get the text from file
    loadStudentsFromFile();
    loadAdminsFromFile();

    // Main menu loop for the system
    int choice;
    while (true) {
        system("cls");
        displayLogo();
        displayMenu();
        choice = validation.inputNumber(3);

        if (choice > 2) {
            cout << "Exiting..." << endl;
            break;
        }


        if (choice == 1) {
            setColor(10); // Green for success messages
            cout << "Student Sign-Up selected." << endl;
            setColor(15); // Reset color
            studentSignUpJSON();
            Sleep(1000);
            continue;
        }

        if (choice == 2) {
            setColor(10); // Green for success messages
            cout << "Log in selected!" << endl;
            setColor(15); // Reset color
            loginProcedure();
            Sleep(1000);
            continue;
        } 
    }
    return 0;
}
