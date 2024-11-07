#include <iostream>
#include <unordered_map>
#include "Display.h"
#include "Class.h"

using namespace std;

// Global storage for students and admins
unordered_map<string, Student> students;
unordered_map<string, Admin> admins;

// Main function
int main() {
    // Get the text from file
    loadStudentsFromFile();
    loadAdminsFromFile();

    // Main menu loop for the system
    int choice;
    while (true) {
        system("cls");
        displayLogo();
        displayMenu();
        cin >> choice;

        if (choice > 2) {
            cout << "Exiting..." << endl;
            break;
        }

        if (choice == 1) {
            setColor(10); // Green for success messages
            cout << "Student Sign-Up selected." << endl;
            setColor(15); // Reset color
            studentSignUpJSON();
            Sleep(400);
            continue;
        }

        string email, password;

        setColor(10); // Green for success messages
        cout << "Sign in selected!" << endl;
        setColor(15); // Reset color
        cout << "Enter email: ";
        cin >> email;
        password = inputPassword();

        if (!(signIn(email, password))) {
            cout << "Incorrect password or email." << endl;
            Sleep(1000);
            continue;
        }

        // Check if the user is a student and show their details
        if (students.find(email) != students.end()) {
            cout << "Login successful!" << endl;
            Sleep(1000);

            // Temp show the student menu to either view details again or log out (sami)/
            int studentChoice;
            bool keepStudentLoggedIn = true;

            while (keepStudentLoggedIn) {
                system("cls");// Clear screen for a fresh menu
                displayLogo();
                setColor(10);
                cout << "\n-- Student Menu --\n";
                setColor(13); // purple color for options
                cout << "1. View My Details\n2. Log Out\n";
                cout << "Choose an option: ";
                cin >> studentChoice;

                switch (studentChoice) {
                case 1:
                    students[email].showDetails();  // Display student details again
                    cout << "\nPress any key to return to the menu...";
                    _getch();  // Wait for the user to press a key , needs some robsut code to replaced with 
                    break;
                case 2:
                    keepStudentLoggedIn = false;  // Log out the student and exit the loop
                    system("cls");
                    displayLogo();  // Show the logo when logging out
                    break;
                default:
                    cout << "Invalid option. Please try again." << endl;
                    Sleep(1000);
                }
            }
            continue;  // Go back to the main menu after logging out /sami
        }

        // Admin login block
        setColor(10); // Green for success messages
        int adminChoice;
        bool keepAdminLoggedIn = true;

        while (keepAdminLoggedIn) {
            cout << "\n-- Admin Menu --\n";
            setColor(12); // Red for admin options
            cout << "1. View All Students\n2. View Domestic Students\n3. View International Students\n4. Remove a Student\n5. Sign Out\n";
            cout << "Choose an admin option: ";
            cin >> adminChoice;

            switch (adminChoice) {
            case 1:
                admins[email].viewAllStudents(students);
                break;
            case 2:
                admins[email].viewDomesticStudents(students);
                break;
            case 3:
                admins[email].viewInternationalStudents(students);
                break;
            case 4: {
                string studentEmail;
               admins[email].viewAllStudentsEmail(students);

                cout << "Enter student email to remove: ";
                cin >> studentEmail;
                admins[email].removeStudent(students, studentEmail);
                break;
            }
            case 5:
                keepAdminLoggedIn = false; // Exit admin menu
                system("cls"); // Clear screen after sign-out
                displayLogo();
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
            }
        }
    }
    return 0;
}
