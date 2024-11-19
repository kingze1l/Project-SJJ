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
    std::string fullName;
    std::string mobile;
    std::string address;
    int age;
    bool isDomestic;
    std::vector<std::string> courses;

public:
    Student() = default;
    Student(std::string email, std::string password, std::string firstName, std::string lastName, std::string fullName, std::string mobile, std::string address, int age, bool isDomestic, std::vector<std::string> courses)
        : User(email, password), firstName(firstName), lastName(lastName), fullName(fullName), mobile(mobile), address(address), age(age), isDomestic(isDomestic), courses(courses) {
    }

    bool getIsDomestic() const;

    void showDetails() const;

    std::vector<std::string> getCourses() const; // Getter for courses

    void removeCourse(const std::string& course); // Allows student to remove courses
    void renewCourse(const std::vector<std::string>& course); // Function to add a course 

    // Getter for firstName
    std::string getFirstName() const {
        return firstName;
    }

    // Getter for lastName
    std::string getLastName() const {
        return lastName;
    }

    // Load student details from a file
    static void loadFromFileJSON(std::ifstream& inFile, std::unordered_map<std::string, Student>& students);
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
    static void loadFromFileJSON(std::ifstream& inFile, std::unordered_map<std::string, Admin>& admins);

    void viewAllStudentsEmail(const std::unordered_map<std::string, Student>& students) const;

    void searchStudentsByName(const std::unordered_map<std::string, Student>& students, const std::string& name) const;
};

extern std::unordered_map<std::string, Student> students;
extern std::unordered_map<std::string, Admin> admins;

// JSON related function to update, remove, and add user data to JSON
void studentSignUpJSON();
void updateStudentCourseJson(std::string email, const std::vector<std::string>& newCourses);
void removeStudentJson(const std::string& email);

// Simple display of user menu and functionality 
void studentMenu(const std::string &email);
void adminMenu(const std::string &email);

// The login procedure for user
void loginProcedure();

// Save data from JSON to system class
void loadStudentsFromFile();
void loadAdminsFromFile();

// User login
bool login(const std::string& email, const std::string& password);
