#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <conio.h>  // For _getch() to hide password input on Windows
#include <sstream>
#include <windows.h>
#include <cstdlib>
#include <regex> // password validation library added by (sami)

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
    cout << "Status: " << (isDomestic ? "Domestic" : "International") << endl;
    cout << "Courses: ";
    for (const auto& course : students[email].getCourses()) {
        cout << course << endl;  // Each course will be printed on a new line
    }
    cout << endl;
}

void Student::addCourse(const string& course) {
    if (courses.size() < 3) {
        courses.push_back(course);
    }
    else {
        cout << "You can only select a maximum of three courses." << endl;
    }
}

void Student::removeCourse(const string& course) {  // funcvtion for student to remove courses 
    auto it = std::find(courses.begin(), courses.end(), course);
    if (it != courses.end()) {
        courses.erase(it);                                                   
    }
    else {
        cout << "Course not found!" << endl;
    }
}

// Implementing the getter for courses so can be used widely (Sami)
vector<string> Student::getCourses() const {
    return courses;
}

void Student::loadFromFileJSON(ifstream& inFile, unordered_map<string, Student>& students) {
    string email, password, firstName, lastName, address;
    int age;
    bool isDomestic;
    vector<string> courses; // array to hold courses for the student 
    json inFiledata;
    inFile >> inFiledata;

    for (auto& i : inFiledata) {
        email = i["email"];
        password = i["password"];
        firstName = i["firstName"];
        lastName = i["lastName"];
        address = i["address"];
        age = i["age"];
        isDomestic = i["isDomestic"];
        for (const auto& course : i["courses"]) {
            courses.push_back(course);
        }
        students[email] = Student(email, password, firstName, lastName, address, age, isDomestic, courses);
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
        cout << counter << ". " << pair.second.getEmail() << endl;
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

vector<string> selectCourses(); // Forward declaration

void studentSignUpJSON() {
    string email, password, firstName, lastName, address;
    int age;
    bool isDomestic;
    vector<string> selectedCourses; // vector to hold the selected courses by the student (sami)
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

    cout << "Is Domestic (1 for Yes, 0 for No): ";
    cin >> isDomestic;

    selectedCourses = selectCourses();

    Student newStudent(email, password, firstName, lastName, address, age, isDomestic, selectedCourses); //created a new student with the selected courses 
    students[email] = newStudent;

    ifstream inputFile("studentnew.json");
    json existingdata;

    if (inputFile.is_open()) {
        inputFile >> existingdata;
        inputFile.close();
    }

    json studentdata{
        {"email", email},
        {"password", password},
        {"firstName", firstName},
        {"lastName", lastName},
        {"address", address},
        {"age", age},
        {"isDomestic", isDomestic},
        {"courses", selectedCourses} // saves the courses as an array in json even though it's a vector (sami)
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

void studentLogin(const string& email) {
    setColor(10);
    cout << "Student login successful!" << endl;
    Sleep(1000);

    int studentChoice;
    bool keepStudentLoggedIn = true;

    while (keepStudentLoggedIn) {
        system("cls");
        displayLogo();
        setColor(10);
        cout << "\n-- Student Menu --\n";
        setColor(13); // Purple color for options
        cout << "1. View My Details\n";
        cout << "2. View My Courses\n";
        cout << "3. Add More Courses\n";
        cout << "4. Remove Course\n";  // New option for removing courses
        cout << "5. Log Out\n";        // Adjusted menu
        cout << "Choose an option: ";
        cin >> studentChoice;

        switch (studentChoice) {
        case 1:
            students[email].showDetails();
            cout << "\nPress any key to return to student menu...";
            _getch();
            break;
        case 2:
            cout << "\nYour Courses: " << endl;
            for (const string& course : students[email].getCourses()) {
                cout << "- " << course << endl;
            }
            cout << "\nPress any key to return to student menu...";
            _getch();
            break;
        case 3: {
            vector<string> newCourses = selectCourses();
            for (const string& course : newCourses) {
                students[email].addCourse(course);
            }
            cout << "Courses added successfully!" << endl;
            Sleep(1000);
            break;
        }
        case 4: {
            // Allow removing a course
            cout << "Select a course to remove:\n";
            const auto& courses = students[email].getCourses();
            for (size_t i = 0; i < courses.size(); ++i) {
                cout << (i + 1) << ". " << courses[i] << endl;
            }
            cout << "Enter the number of the course to remove (or 0 to cancel): ";
            int courseChoice;
            cin >> courseChoice;

            if (courseChoice > 0 && courseChoice <= courses.size()) {
                string courseToRemove = courses[courseChoice - 1];
                students[email].removeCourse(courseToRemove);
                cout << "Course \"" << courseToRemove << "\" removed successfully!" << endl;
            }
            else if (courseChoice == 0) {
                cout << "No course removed." << endl;
            }
            else {
                cout << "Invalid selection." << endl;
            }
            Sleep(1000);
            break;
        }
        case 5:
            cout << "Logging out...";
            keepStudentLoggedIn = false;
            break;
        default:
            cout << "Invalid option. Please try again." << endl;
            Sleep(1000);
        }
    }
}

void adminLogin(string& email) {
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

// function to allow the student to select courses
vector<string> selectCourses() {
    vector<string> availableCourses = { "Certificate in Creative Media", "Diploma in Digital Design – Web and Graphic Design", "Bachelor of Software Engineering", "Diploma in Creative Digital Design", "Diploma in Software Development", "Diploma in Film and Content Creation" };
    vector<string> selectedCourses;
    int courseChoice;

    cout << "Select your first course (1-6): ";
    // Display available courses to the student
    for (int i = 0; i < availableCourses.size(); ++i) {
        cout << (i + 1) << ". " << availableCourses[i] << endl;
    }

    cin >> courseChoice;
    if (courseChoice < 1 || courseChoice > availableCourses.size()) {
        cout << "Invalid course selection!" << endl;
        return selectedCourses;  // Return empty vector if invalid    
    }

    // Adding the first selected course
    selectedCourses.push_back(availableCourses[courseChoice - 1]);

    // Allow student to choose up to two more courses
    while (selectedCourses.size() < 3) {
        cout << "Select another course (or enter 0 to stop selecting): ";
        cin >> courseChoice;
        if (courseChoice == 0) {
            break;  // Stop selecting if user choice is 0        
        }
        if (courseChoice < 1 || courseChoice > availableCourses.size()) {
            cout << "Invalid course selection!" << endl;
            continue;
        }

        // making sure no duplicates are recorded
        if (find(selectedCourses.begin(), selectedCourses.end(), availableCourses[courseChoice - 1]) != selectedCourses.end()) {
            cout << "You have already selected this course!" << endl;
        }
        else {
            selectedCourses.push_back(availableCourses[courseChoice - 1]);
        }
    }
    return selectedCourses;
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
    // not really needed but makes life a whole lot easier, code more readable
    if (admins.find(email) != admins.end()) {
        adminLogin(email);
        return;
    }

    cout << "System failed to detect whether the email belongs to the user or an admin" << endl;
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
