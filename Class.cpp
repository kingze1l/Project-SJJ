#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <conio.h>  // For _getch() to hide password input on Windows
#include <sstream>
#include <windows.h>
#include <cstdlib> 
#include <regex> // password validation libary added by (sami)

#include "nlohmann/json.hpp"
#include "Display.h"
#include "Class.h"
#include "validation.h"

using namespace std;
using namespace nlohmann;

// USER
bool User::login(string inputEmail, string inputPassword) {
    return (email == inputEmail && password == inputPassword);
}

string User::getEmail() const {
    return email;
}

// STUDENT 

bool Student::getIsDomestic() const {
    return isDomestic;
}

void Student::showDetails() const {
    cout << "Student's Name: " << firstName << " " << lastName << endl;
    cout << "Student's Age: " << age << endl;
    cout << "Student's Address: " << address << endl;
    cout << "Email: " << email << endl;
    cout << "Course: " << course << endl;
    cout << "Status: " << (isDomestic ? "Domestic" : "International") << endl;
}

void Student::loadFromFileJSON(ifstream& inFile, unordered_map<string, Student>& students) {
    string email, password, firstName, lastName, course, address;
    int age;
    bool isDomestic;

    json inFiledata;
    inFile >> inFiledata;

    for (auto& i : inFiledata) {
        email = i["email"];
        password = i["password"];
        firstName = i["firstName"];
        lastName = i["lastName"];
        course = i["course"];
        address = i["address"];
        age = i["age"];
        isDomestic = i["isDomestic"];
        students[email] = Student(email, password, firstName, lastName, course, address, age, isDomestic);
    }

}

// ADMIN
void Admin::viewAllStudents(const unordered_map<string, Student>& students) const {
    cout << "\n-- All Students --" << endl;
    for (const auto& pair : students) {
        pair.second.showDetails();
        cout << "-----------------" << endl;
    }
}

void Admin::viewDomesticStudents(const unordered_map<string, Student>& students) const {
    cout << "\n-- Domestic Students --" << endl;
    for (const auto& pair : students) {
        if (pair.second.getIsDomestic()) {
            pair.second.showDetails();
            cout << "-----------------" << endl;
        }
    }
}

void Admin::viewInternationalStudents(const unordered_map<string, Student>& students) const {
    cout << "\n-- International Students --" << endl;
    for (const auto& pair : students) {
        if (!pair.second.getIsDomestic()) {
            pair.second.showDetails();
            cout << "-----------------" << endl;
        }
    }
}

void Admin::viewAllStudentsEmail(const std::unordered_map<std::string, Student>& students) const {
    int counter{};
    cout << "\n-- All Students' Email --" << endl;
    for (const auto& pair : students) {
        counter++;
        cout << counter <<". " << pair.second.getEmail() << endl;
    }
    cout << "-----------------" << endl;
}

void Admin::removeStudent(unordered_map<string, Student>& students, string studentEmail) const {
    if (students.find(studentEmail) != students.end()) {
        students.erase(studentEmail);
        cout << "Student with email " << studentEmail << " has been removed." << endl;
    }
    else {
        cout << "Student not found." << endl;
    }
}

void Admin::loadFromFileJSON(ifstream& inFile, unordered_map<string, Admin>& admins) {
    string email, password;

    json inFiledata;
    inFile >> inFiledata;
    
    for (auto& i : inFiledata) {
        email = i["email"];
        password = i["password"];
        admins[email] = Admin(email, password);
    }
}

// TEMP: Is there a need for this?
//bool isValidMobile(const string& mobile) { // function for validating the mobile number
//    regex pattern(R"(\d{10})");
//    return regex_match(mobile, pattern);
//}

void studentSignUpJSON() {
    string email, password, firstName, lastName, course, address;
    int age;
    bool isDomestic;
    Validation validation;
    
    cout << "Enter student details for sign-up:" << endl;
    email = validation.inputEmailValidation();
    password = validation.inputPasswordValidation();

    cout << "First Name: ";
    cin >> firstName;
    cout << "Last Name: ";
    cin >> lastName;
    cout << "Age: ";

    age = validation.inputAgeValidation();
    address = validation.inputAddress();

    cout << "Course: ";
    getline(cin, course);

    cout << "Is Domestic (1 for Yes, 0 for No): ";
    cin >> isDomestic;

    Student newStudent(email, password, firstName, lastName, course, address, age, isDomestic);
    students[email] = newStudent;

    ifstream inputFile("studentnew.json");
    json existingdata;

    if (inputFile.is_open()) {
        inputFile >> existingdata;
        inputFile.close();
    }

    json studentdata{
        {"email",email},
        {"password",password},
        {"firstName",firstName},
        {"lastName",lastName},
        {"course", course},
        {"address",course},
        {"age",age},
        {"isDomestic", isDomestic},
    };
    existingdata.push_back(studentdata);

    ofstream outFile("studentnew.json");
    if (outFile.is_open()) {
        
        outFile << existingdata.dump(4);
        outFile.close();
        cout << "Student registered and saved successfully!" << endl;
    }
    else {
        cout << "Error saving student to file." << endl;
    }
}

void studentLogin(const string &email) {
    setColor(10);
    cout << "Student login successful!" << endl;
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
            cout << "\nPress any key to return to student menu...";
            _getch();  // Wait for the user to press a key , needs some robsut code to replaced with 
            break;
        case 2:
            cout << "Logging out...";
            keepStudentLoggedIn = false;  // Log out the student and exit the loop
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
            Sleep(1000);
        }
    }
}

void adminLogin(string &email) {
    setColor(12);
    cout << "Admin login successful!" << endl;
    Sleep(1000);

    
    int adminChoice;
    bool keepAdminLoggedIn = true;

    while (keepAdminLoggedIn) {
        system("cls");
        displayLogo();
        
        cout << "\n-- Admin Menu --\n";
        setColor(12); // Red for admin options
        cout << "1. View All Students\n2. View Domestic Students\n3. View International Students\n4. Remove a Student\n5. Sign Out\n";
        cout << "Choose an admin option: ";
        cin >> adminChoice;

        switch (adminChoice) {
        case 1:
            system("cls");
            admins[email].viewAllStudents(students);
            cout << "\nPress any key to return to admin menu...";
            _getch();
            break;
        case 2:
            system("cls");
            admins[email].viewDomesticStudents(students);
            cout << "\nPress any key to return to admin menu...";
            _getch();
            break;
        case 3:
            system("cls");
            admins[email].viewInternationalStudents(students);
            cout << "\nPress any key to return to admin menu...";
            _getch();
            break;
        case 4: {
            system("cls");
            string studentEmail;
            admins[email].viewAllStudentsEmail(students);

            cout << "Enter student email to remove: ";
            cin.clear(); cin.ignore(1000, '\n');
            getline(cin, studentEmail);
            admins[email].removeStudent(students, studentEmail);
            Sleep(1000);
            break;
        }
        case 5:
            cout << "Logging out...";
            keepAdminLoggedIn = false; // Exit admin menu
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
        }
    }
}

void signInProcedure() {
    Validation v;
    string email, password;
    
    cin.clear(); cin.ignore(1000, '\n');
    while (true) {
        cout << "Enter email: ";
        getline(cin, email);
        
        password = v.inputPassword();

        if (signIn(email, password)) {
            break;
        }

        cout << "Incorrect password or email, please enter again!" << endl;
    }

    // Check if the user is a student and show their details
    if (students.find(email) != students.end()) {
        studentLogin(email);
        return;
    }
    // not really needed but make life a whole lot easier, code more readable
    if (admins.find(email) != admins.end()) {
        adminLogin(email);
        return;
    }

    cout << "System failed to detect whether the email belong to the user or an admin" << endl;
    return;
}

// Function to load students from a file
void loadStudentsFromFile() {
    ifstream inFile("studentnew.json");
    if (inFile.is_open()) {
        Student::loadFromFileJSON(inFile, students);
        inFile.close();
    }
}

// Function to load admins from a file
void loadAdminsFromFile() {
    ifstream inFile("new_admins.json");
    if (inFile.is_open()) {
        Admin::loadFromFileJSON(inFile, admins);
        inFile.close();
    }
}

// Sign-in function
bool signIn(const string& email, const string& password) {
    if (admins.find(email) != admins.end()) {
        if (admins[email].login(email, password)) {
            setColor(10);
            return true;
        }
    }

    if (students.find(email) != students.end()) {
        if (students[email].login(email, password)) {
            return true;
        }
    }
    return false;
}


