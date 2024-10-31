#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <conio.h>  // For _getch() to hide password input on Windows
#include <sstream>
#include <windows.h>
#include <cstdlib> 

// Base User class
class User {
protected:
    std::string email;
    std::string password;
public:
    User() = default;
    User(std::string email, std::string password) : email(email), password(password) {}

    virtual bool login(std::string inputEmail, std::string inputPassword);

    std::string getEmail() const;
};


// //Derived Student class
class Student : public User {
private:
    std::string firstName;
    std::string lastName;
    std::string course;
    bool isDomestic;
public:
    Student() = default;
    Student(std::string email, std::string password, std::string firstName, std::string lastName, std::string course, bool isDomestic)
        : User(email, password), firstName(firstName), lastName(lastName), course(course), isDomestic(isDomestic) {}

    bool getIsDomestic() const;

    void showDetails() const;

    // Save student details to a file
    void saveToFile(std::ofstream& outFile) const;

    // Load student details from a file
    static void loadFromFile(std::ifstream& inFile, std::unordered_map<std::string, Student>& students);

};


// Derived Admin class
class Admin : public User {
public:
    Admin() = default;
    Admin(std::string email, std::string password) : User(email, password) {}

    void viewAllStudents(const std::unordered_map<std::string, Student>& students) const;

    void viewDomesticStudents(const std::unordered_map<std::string, Student>& students) const;

    void viewInternationalStudents(const std::unordered_map<std::string, Student>& students) const;

    void removeStudent(std::unordered_map<std::string, Student>& students, std::string studentEmail) const;

    // Load admin details from a file
    static void loadFromFile(std::ifstream& inFile, std::unordered_map<std::string, Admin>& admins);
};

extern std::unordered_map<std::string, Student> students;
extern std::unordered_map<std::string, Admin> admins;

// Function to hide password input
std::string inputPassword();

// Function to sign up a student and save details to a file
void studentSignUp();

// Function to load students from a file
void loadStudentsFromFile();

// Function to load admins from a file
void loadAdminsFromFile();

// Sign-in function
bool signIn(const std::string& email, const std::string& password);
