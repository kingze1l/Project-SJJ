#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <conio.h>  // For _getch() to hide password input on Windows
#include <sstream>
#include <windows.h>
#include <cstdlib> 

#include "Class.h"

using namespace std;

bool User::login(string inputEmail, string inputPassword) {
    return (email == inputEmail && password == inputPassword);
}

string User::getEmail() const {
    return email;
}


// Derived Student class
//class Student : public User {
//private:
//    string firstName;
//    string lastName;
//    string course;
//    bool isDomestic;
//public:
//    Student() = default;
//    Student(string email, string password, string firstName, string lastName, string course, bool isDomestic)
//        : User(email, password), firstName(firstName), lastName(lastName), course(course), isDomestic(isDomestic) {}
//
//    bool getIsDomestic() const {
//        return isDomestic;
//    }
//
//    void showDetails() const {
//        cout << "Student Name: " << firstName << " " << lastName << endl;
//        cout << "Email: " << email << endl;
//        cout << "Course: " << course << endl;
//        cout << "Status: " << (isDomestic ? "Domestic" : "International") << endl;
//    }
//
//    // Save student details to a file
//    void saveToFile(ofstream& outFile) const {
//        outFile << email << " " << password << " " << firstName << " " << lastName << " "
//            << course << " " << isDomestic << endl;
//    }
//
//    // Load student details from a file
//    static void loadFromFile(ifstream& inFile, unordered_map<string, Student>& students) {
//        string email, password, firstName, lastName, course;
//        bool isDomestic;
//        while (inFile >> email >> password >> firstName >> lastName >> course >> isDomestic) {
//            students[email] = Student(email, password, firstName, lastName, course, isDomestic);
//        }
//    }
//};


//// Derived Admin class
//class Admin : public User {
//public:
//    Admin() = default;
//    Admin(string email, string password) : User(email, password) {}
//
//    void viewAllStudents(const unordered_map<string, Student>& students) const {
//        cout << "\n-- All Students --" << endl;
//        for (const auto& pair : students) {
//            pair.second.showDetails();
//            cout << "-----------------" << endl;
//        }
//    }
//
//    void viewDomesticStudents(const unordered_map<string, Student>& students) const {
//        cout << "\n-- Domestic Students --" << endl;
//        for (const auto& pair : students) {
//            if (pair.second.getIsDomestic()) {
//                pair.second.showDetails();
//                cout << "-----------------" << endl;
//            }
//        }
//    }
//
//    void viewInternationalStudents(const unordered_map<string, Student>& students) const {
//        cout << "\n-- International Students --" << endl;
//        for (const auto& pair : students) {
//            if (!pair.second.getIsDomestic()) {
//                pair.second.showDetails();
//                cout << "-----------------" << endl;
//            }
//        }
//    }
//
//    void removeStudent(unordered_map<string, Student>& students, string studentEmail) const {
//        if (students.find(studentEmail) != students.end()) {
//            students.erase(studentEmail);
//            cout << "Student with email " << studentEmail << " has been removed." << endl;
//        }
//        else {
//            cout << "Student not found." << endl;
//        }
//    }
//
//    // Load admin details from a file
//    static void loadFromFile(ifstream& inFile, unordered_map<string, Admin>& admins) {
//        string email, password;
//        while (inFile >> email >> password) {
//            admins[email] = Admin(email, password);
//        }
//    }
//};