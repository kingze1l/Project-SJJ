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
    std::string course;
    std::string address;
    int age;
    bool isDomestic;
    std::vector<std::string> courses;

public:
    Student() = default;
    Student(std::string email, std::string password, std::string firstName, std::string lastName, std::string fullName, std::string mobile, std::string address, int age, bool isDomestic , std::vector<std::string> courses)
        : User(email, password), firstName(firstName), lastName(lastName), fullName(fullName), mobile(mobile), address(address), age(age), isDomestic(isDomestic) , courses(courses) {}

    bool getIsDomestic() const;

    void showDetails() const;

    std::vector<std::string> getCourses() const; // this is a getter for courses. (sami)
    void removeCourse(const std::string& course); // added so we student can remove courses
    void renewCourse(const std::vector<std::string>& course); // function to add a course 

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
};

extern std::unordered_map<std::string, Student> students;
extern std::unordered_map<std::string, Admin> admins;

// Function to sign up a student and save details to a file
void studentSignUpJSON();

void studentLogin(const std::string &email);
void adminLogin(const std::string &email);
void signInProcedure();

// Function to load students from a file
void loadStudentsFromFile();

// Function to load admins from a file
void loadAdminsFromFile();

// Sign-in function
bool signIn(const std::string& email, const std::string& password);
