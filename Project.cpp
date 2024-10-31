
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
    // Display TalentHub logo at start
    displayLogo();
    loadStudentsFromFile();
    loadAdminsFromFile();

    // Main menu loop for the system
    int choice;
    while (true) {
       
        displayMenu();
        cin >> choice;

        if (choice == 1) {
            setColor(10); // Green for success messages
            cout << "Student Sign-Up selected." << endl;
            setColor(15); // Reset color
            studentSignUp();
        }
        else if (choice == 2) {
            string email, password;
            cout << "Enter email: ";
            cin >> email;
            password = inputPassword();
            if (signIn(email, password)) { // can use better naming convention ableToSignIn()
                if (admins.find(email) != admins.end()) { // nested if statement is bad practice
                    setColor(10);
                    int adminChoice;
                    bool keepAdminLoggedIn = true;
                    while (keepAdminLoggedIn) { // this whole while loop can be put inside another function such as adminMenu()
                        cout << "\n-- Admin Menu --\n";
                        setColor(12);
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
            }
        }
        else if (choice == 3) {
            break; // Exit program
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}