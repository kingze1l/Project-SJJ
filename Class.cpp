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
    cout << "Courses: \n";
    int i=1;
    for (const auto& course : students[email].getCourses()) {
        cout << i++ << course << endl;  // Each course will be printed on a new line
    }
    cout << endl;
}

void Student::renewCourse(const vector<string>& selectedCourses) {
    courses = selectedCourses;
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

        courses = {};
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

void removeStudentJson(const string& email);
void Admin::removeStudent(unordered_map<string, Student>& students, string studentEmail) const {
    if (students.find(studentEmail) != students.end()) {
        students.erase(studentEmail);
        removeStudentJson(studentEmail);
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

vector<string> selectCourses(const vector<string>& studentCourses = {}); // Forward declaration

void updateStudentCourseJson(string email, const vector<string>& newCourses) {
    ifstream inFile("studentnew.json");

    if (!(inFile.is_open())) {
        cout << "ERROR: CAN'T OPEN STUDENT JSON addStudentCourseJson()" << endl;
        return;
    }

    json jsonArray;
    inFile >> jsonArray;
    for (auto& i : jsonArray) {
        if (i["email"] == email) {
            i["courses"] = newCourses;
            break;
        }
    }

    ofstream outFile("studentnew.json");
    if (!(outFile.is_open())) {
        cout << "ERROR: CAN'T OPEN STUDENT JSON updateStudentCourseJson()" << endl;
        return;
    }
    outFile << jsonArray.dump(4);
}

void removeStudentJson(const string& email) {
    ifstream inFile("studentnew.json");
    if (!inFile.is_open()) {
        cout << "ERROR: CAN'T OPEN STUDENT JSON addStudentCourseJson()" << endl;
        return;
    }
    json jsonArray;

    inFile >> jsonArray;
    for (auto it = jsonArray.begin(); it != jsonArray.end();) {
        if ((*it)["email"] == email) {
            it = jsonArray.erase(it);
        } else{ 
            it++;
        }
    }

    ofstream outFile("studentnew.json");
    if (!(outFile.is_open())) {
        cout << "ERROR: CAN'T OPEN STUDENT JSON updateStudentCourseJson()" << endl;
        return;
    }
    outFile << jsonArray.dump(4);
}
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
    if (!(inputFile.is_open())) {
        cout << "ERROR: STUDENT JSON CAN'T BE OPENED";
        return;
    }

    json existingdata{};
    if (!(inputFile.peek() == ifstream::traits_type::eof())) {
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
   
    if (!outFile.is_open()) {
        cout << "ERROR: CAN'T OPEN STUDENT JSON FILE, studentSignInJson()";
        Sleep(1000);
        return;
    }

    outFile << existingdata.dump(4);
    outFile.close();
    cout << "Student registered and saved successfully!" << endl;
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
            if (students[email].getCourses().size() >= 3) {
                cout << "You already have 3 course, please remove a course to add other course";
                cout << "\nPress any key to return to student menu...";
                _getch();
                break;
            }

            vector<string> newCourses = selectCourses(students[email].getCourses());
            students[email].renewCourse(newCourses);
            updateStudentCourseJson(students[email].getEmail(), newCourses);
            
            cout << "Courses added successfully!" << endl;
            cout << "\nPress any key to return to student menu...";
            _getch();
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
                updateStudentCourseJson(students[email].getEmail(), students[email].getCourses());
                cout << "Course \"" << courseToRemove << "\" removed successfully!" << endl;
            }
            else if (courseChoice == 0) {
                cout << "No course removed." << endl;
            }
            else {
                cout << "Invalid selection." << endl;
            }
            cout << "\nPress any key to return to student menu...";
            _getch();
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
vector<string> selectCourses(const vector<string> &studentCourses) {
    Validation v;
    vector<string> availableCourses = { 
        "Certificate in Creative Media", 
        "Diploma in Digital Design - Web and Graphic Design", 
        "Bachelor of Software Engineering", 
        "Diploma in Creative Digital Design", 
        "Diploma in Software Development", 
        "Diploma in Film and Content Creation" 
    };

    vector<string> selectedCourses = studentCourses;
    int courseChoice{};

    if (!selectedCourses.empty()) {
        cout << "Your current course: " << endl;
        for (int i = 0; i < selectedCourses.size(); i++){
            cout << i + 1 << ". " << selectedCourses[i] << endl;
        }
    }
    cout << endl;

    cout << "Select a course (1-6): \n";
    for (int i = 0; i < availableCourses.size(); ++i) {
        cout << (i + 1) << ". " << availableCourses[i] << endl;
    }

    int take = 0;
    while (selectedCourses.size() < 3) {
        if (take > 0) {
            cout << "Select another course (or enter 0 to stop selecting): ";
        }
        else {
            cout << "Please select a course: ";
        }
        
        courseChoice = v.inputNumber(availableCourses.size());

        if (courseChoice == 0) {
            break;     
        }
   
        if (find(selectedCourses.begin(), selectedCourses.end(), availableCourses[courseChoice - 1]) != selectedCourses.end()) {
            cout << "You have already selected this course!" << endl;
            continue;
        }
        take++;
        selectedCourses.push_back(availableCourses[courseChoice - 1]);
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
    if (inFile.peek() == ifstream::traits_type::eof()) {
        cout << "STUDENT JSON IS EMPTY";
        Sleep(1000);
        return;
    }

    if (!(inFile.is_open()) ){
        cout << "STUDENT JSON CAN'T BE OPENED";
        Sleep(1000);
        return;
    }

    Student::loadFromFileJSON(inFile, students);
    inFile.close();
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
