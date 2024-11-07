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
#include <regex> // password validation libary added by (sami)
#include "nlohmann/json.hpp"

using namespace std;
using namespace nlohmann;

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

bool isValidAge(int age) {
    return age >= 18;
}

bool isValidMobile(const string& mobile) { // function for validating the mobile number
    regex pattern(R"(\d{10})");
    return regex_match(mobile, pattern);
}

// function for password complexity
bool validatePassword(const string& password) {
   // regex pattern for password validation
    regex pattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#$%^&*()-_+=])(?=\\S+$).{8,}");
    // checking password mattches the pattern 
    return regex_match(password, pattern);
}

// function for validating email format                           added by sami 
bool validateEmail(const string& email) {
    regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
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
string inputAddress() {
    string address{};
    cout << "Address: ";
    cin.clear();
    cin.ignore();
    getline(cin, address);

    return address;
}

void studentSignUpJSON() {
    string email, password, firstName, lastName, course, address;
    int age;
    bool isDomestic;
    
    cout << "Enter student details for sign-up:" << endl;

    // Validating email format and ensuring it's unique
    while (true) {
        cout << "Email: ";
        cin >> email;
        if (!validateEmail(email)) {
            cout << "Invalid email format. Please try again." << endl;
            continue;
        }

        if (students.find(email) == students.end()) {
            break;  // If email is unique, break the loop
        } else {
            cout << "Email already exists. Please enter a new, different email." << endl;
        }
    }

    // Validating password (password must meet certain criteria)
    while (true) {
        password = inputPassword();
        if (validatePassword(password)) {
            break;
        } else {
            cout << "Password is invalid. Please ensure it meets the following criteria:" << endl;
            cout << "- At least 8 characters long" << endl;
            cout << "- Contains at least one uppercase letter" << endl;
            cout << "- Contains at least one lowercase letter" << endl;
            cout << "- Contains at least one digit" << endl;
            cout << "- Contains at least one special character (e.g., @, #, $, etc.)" << endl;
        }
    }
    cout << "First Name: ";
    cin >> firstName;
    cout << "Last Name: ";
    cin >> lastName;
    cout << "Age: ";
    cin >> age;

    address = inputAddress();

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
            cout << "Admin login successful!" << endl;
            return true;
        }
    }

    if (students.find(email) != students.end()) {
        if (students[email].login(email, password)) {
            cout << "Student login successful!" << endl;
            return true;
        }
    }
    return false;
}


