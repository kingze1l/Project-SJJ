#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <conio.h>  // For _getch() to hide password input on Windows
#include <sstream>
#include <windows.h>
#include <cstdlib> 
#include "Display.h"
#include "Class.h"

using namespace std;

bool User::login(string inputEmail, string inputPassword) {
    return (email == inputEmail && password == inputPassword);
}

string User::getEmail() const {
    return email;
}



bool Student::getIsDomestic() const {
    return isDomestic;
}

void Student::showDetails() const {
    cout << "Student Name: " << firstName << " " << lastName << endl;
    cout << "Email: " << email << endl;
    cout << "Course: " << course << endl;
    cout << "Status: " << (isDomestic ? "Domestic" : "International") << endl;
}

void Student::saveToFile(ofstream& outFile) const {
    outFile << email << " " << password << " " << firstName << " " << lastName << " "
        << course << " " << isDomestic << endl;
}

void Student::loadFromFile(ifstream& inFile, unordered_map<string, Student>& students) {
    string email, password, firstName, lastName, course;
    bool isDomestic;
    while (inFile >> email >> password >> firstName >> lastName >> course >> isDomestic) {
        students[email] = Student(email, password, firstName, lastName, course, isDomestic);
    }
}



// Derived Admin class
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

void Admin::removeStudent(unordered_map<string, Student>& students, string studentEmail) const {
    if (students.find(studentEmail) != students.end()) {
        students.erase(studentEmail);
        cout << "Student with email " << studentEmail << " has been removed." << endl;
    }
    else {
        cout << "Student not found." << endl;
    }
}



// Load admin details from a file
void Admin::loadFromFile(ifstream& inFile, unordered_map<string, Admin>& admins) {
    string email, password;
    while (inFile >> email >> password) {
        admins[email] = Admin(email, password);
    }
}



// Function to hide password input
string inputPassword() {
    string password;
    char ch;
    cout << "Enter password: ";
    while ((ch = _getch()) != '\r') { // '\r' is Enter key in Windows
        if (ch == '\b' && !password.empty()) { // Handle backspace
            password.pop_back();
            cout << "\b \b";
        }
        else if (ch != '\b') {
            password.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

// Function to sign up a student and save details to a file
void studentSignUp() {
    string email, password, firstName, lastName, course;
    bool isDomestic;

    cout << "Enter student details for sign-up:" << endl;
    cout << "Email: ";
    cin >> email;
    password = inputPassword();
    cout << "First Name: ";
    cin >> firstName;
    cout << "Last Name: ";
    cin >> lastName;
    cout << "Course: ";
    cin >> course;
    cout << "Is Domestic (1 for Yes, 0 for No): ";
    cin >> isDomestic;

    Student newStudent(email, password, firstName, lastName, course, isDomestic);
    students[email] = newStudent;

    ofstream outFile("students.txt", ios::app);
    if (outFile.is_open()) {
        newStudent.saveToFile(outFile);
        outFile.close();
        cout << "Student registered and saved successfully!" << endl;
    }
    else {
        cout << "Error saving student to file." << endl;
    }
}

// Function to load students from a file
void loadStudentsFromFile() {
    ifstream inFile("students.txt");
    if (inFile.is_open()) {
        Student::loadFromFile(inFile, students);
        inFile.close();
    }
}

// Function to load admins from a file
void loadAdminsFromFile() {
    ifstream inFile("admins.txt");
    if (inFile.is_open()) {
        Admin::loadFromFile(inFile, admins);
        inFile.close();
    }
}

// Sign-in function
bool signIn(const string& email, const string& password) {
    if (admins.find(email) != admins.end()) {
        if (admins[email].login(email, password)) {
            setColor(10);
            cout << "Admin login successful!" << endl;
            return true;
        }
        else {
            setColor(12);
            cout << "Incorrect password for admin." << endl;
            return false;
        }
    }

    if (students.find(email) != students.end()) {
        if (students[email].login(email, password)) {
            cout << "Student login successful!" << endl;
            return true;
        }
        else {
            cout << "Incorrect password for student." << endl;
            return false;
        }
    }

    cout << "Email not found." << endl;
    return false;
}


