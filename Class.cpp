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
#include "encryption.h"

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
    cout << left << setw(30) << "Student's Name " << setw(10) << '|' << fullName << endl;
    cout << left << setw(30) << "Student's Age " << setw(10) << '|' << age << endl;
    cout << left << setw(30) << "Student's Address " << setw(10) << '|' << address << endl;
    cout << left << setw(30) << "Student's Mobile Number "  << setw(10) << '|' << mobile << endl;
    cout << left << setw(30) << "Email " << setw(10) << '|' << email << endl;
    cout << left << setw(30) << "Status " << setw(10) << '|' << (isDomestic ? "Domestic" : "International") << endl;
    if (courses.empty()) {
        cout << left << setw(30) << "Courses " << setw(10) << '|' << "No Courses" << endl << endl;
        return;
    }

    cout << left << setw(30) << "Courses" << setw(10) << '|' << "1. " << courses[0] << endl;

    for (int i = 1; i < courses.size(); i++) {
        cout << left << setw(30) << ' ' << setw(10) << '|' << i + 1 << ". " << courses[i] << endl;  // Each course will be printed on a new line
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
    string email, password, firstName, lastName, fullName, mobile, address;
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
        fullName = i["fullName"];
        mobile = i["mobile"];
        address = i["address"];
        age = i["age"];
        isDomestic = i["isDomestic"];

        courses = {};
        for (const auto& course : i["courses"]) {
            courses.push_back(course);
        }
        students[email] = Student(email, password, firstName, lastName, fullName, mobile, address, age, isDomestic, courses);
    }
}

// ADMIN
void Admin::viewAllStudents(const unordered_map<string, Student>& students) const {
    cout << "All Students" << endl;
    cout << string(100, '-') << endl;
    for (const auto& pair : students) {
        cout << endl;
        pair.second.showDetails();
        cout << string(100, '-') << endl;
    }
}

void Admin::viewDomesticStudents(const unordered_map<string, Student>& students) const {
    cout << "Domestic Students" << endl;
    cout << string(100, '-') << endl;
    for (const auto& pair : students) {
        if (pair.second.getIsDomestic()) {
            cout << endl;
            pair.second.showDetails();
            cout << string(100, '-') << endl;
        }
    }
}

void Admin::viewInternationalStudents(const unordered_map<string, Student>& students) const {
    cout << "International Students" << endl;
    cout << string(100, '-') << endl;
    for (const auto& pair : students) {
        if (!pair.second.getIsDomestic()) {
            cout << endl;
            pair.second.showDetails();
            cout << string(100, '-') << endl;
        }
    }
}

void Admin::viewAllStudentsEmail(const unordered_map<string, Student>& students) const {
    int counter{};
    cout << "\n-- All Students' Email --" << endl;
    for (const auto& pair : students) {
        counter++;
        cout << counter << ". " << pair.second.getEmail() << endl;
    }
    cout << "-----------------" << endl;
}

void Admin::viewAllStudentsNameandEmail(const unordered_map<string, Student>& students) const {
    int counter = 1;
    cout << "\n--All Students' Name and Email --" << endl;
    for (const auto& pair : students) {
        cout << counter++ << ". " <<  "Email: " << left << setw(25) << pair.second.getEmail() <<  " | Full name: " << pair.second.getFirstName() + " " + pair.second.getLastName() << endl;
    }
}

void Admin::searchStudentsByName(const unordered_map<string, Student>& students, const std::string& name) const {
    cout << "\n-- Search Results for \"" << name << "\" --" << endl;
    bool found = false;
    
    for (const auto& pair : students) {
        const Student& student = pair.second;
        if (student.getFirstName() == name || student.getLastName() == name) {
            student.showDetails();
            cout << "-----------------" << endl;
            found = true;
        }
    }
    
    if (!found) {
        cout << "No students found with the name \"" << name << "\"." << endl;
    }
}


void removeStudentJson(const string& email);
void Admin::removeStudent(unordered_map<string, Student>& students, string studentEmail) {
    students.erase(studentEmail);
    removeStudentJson(studentEmail);
    setColor(10);  // Light Green for success message
    cout << "Student is removed successfully" << endl;
}

void Admin::removeStudentCourse(unordered_map<string, Student>& students, string studentEmail) {
    Validation v;
    cout << "Select a course to remove from: " << students[studentEmail].getFirstName() << endl;
    vector<string> courses = students[studentEmail].getCourses();
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << (i + 1) << ". " << courses[i] << endl;
    }
    cout << "Enter the number of the course to remove (or 0 to cancel): ";

    int courseChoice{};
    courseChoice = v.inputNumber(courses.size());

    if (courseChoice > 0 && courseChoice <= courses.size()) {
        string courseToRemove = courses[courseChoice - 1];
        students[studentEmail].removeCourse(courseToRemove);
        updateStudentCourseJson(students[studentEmail].getEmail(), students[studentEmail].getCourses());

        setColor(10);  // Light Green for success message
        cout << "Course \"" << courseToRemove << "\" removed successfully!" << endl;
    }
    else if (courseChoice == 0) {
        setColor(15);  // White for no action
        cout << "No course removed." << endl;
    }
    else {
        setColor(12);  // Red for invalid selection
        cout << "Invalid selection." << endl;
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
    string email, password, firstName, lastName, mobile, fullName, address;
    int age;
    bool isDomestic;
    vector<string> selectedCourses; // vector to hold the selected courses by the student (sami)
    Validation validation;

    cout << "Enter student details for sign-up:" << endl;
    email = validation.inputEmailValidation();
    if (email == "") {
        cout << "Returning to main menu..." << endl;
        return;
    }

    // ENCRYPTION

    encrypt::Encryption encryptClass;
    const uint8_t privateKey[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
    0xab, 0xf7, 0x97, 0x75, 0x46, 0x65, 0x1d, 0x37};

    vector<uint8_t> encrypted{};
    
    string originalPassword{}, confirmPassword{};
    while (true) { 
        originalPassword = validation.inputPasswordValidation();
        cout << "Please Confirm Password" << endl;
        confirmPassword = validation.inputPassword();
        if (originalPassword != confirmPassword) {
            cout << "Password do not match, please insert again!" << endl;
            continue;
        } break;
    }

    encryptClass.encryptPassword(
        originalPassword.c_str(),
        privateKey,
        encrypted
    );


    password = encryptClass.symbolToHex(encrypted);


    cout << "First Name: ";
    getline(cin, firstName);
    cout << "Last Name: ";
    getline(cin, lastName);

    fullName = firstName + " " + lastName;

    cout << "Age: ";
    age = validation.inputAgeValidation();

    validation.discardExtraInput();
    while (true) {
        cout << "Mobile Number: ";
        getline(cin, mobile);

        if (Validation::isValidNZMobileNumber(mobile)) {  // Call static method directly
            break;  // If the mobile number is valid, break the loop
        }
        else {
            cout << "Invalid mobile number. Please enter a valid NZ mobile number." << endl;
        }
    }

    address = validation.inputAddress();

    cout << "Are you a domestic student? (1 for Yes, 0 for No): ";
    isDomestic = validation.inputNumber(1);

    selectedCourses = selectCourses();

    Student newStudent(email, password, firstName, lastName, fullName, mobile, address, age, isDomestic, selectedCourses); //created a new student with the selected courses 
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
        {"fullName", fullName},
        {"mobile", mobile},
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
    setColor(10);
    cout << "Student registered and saved successfully!" << endl;
}

void studentMenu(const string& email) {
    Validation v;
    setColor(10);
    cout << "Student login successful!" << endl;
    Sleep(1000);

    int studentChoice;
    bool keepStudentLoggedIn = true;

    while (keepStudentLoggedIn) {
        system("cls");
        displayLogo();
        setColor(9);
        cout << "\n-- Student Menu --\n";
        setColor(10); // Purple color for options
        cout << "1. View My Details\n";
        cout << "2. View My Courses\n";
        cout << "3. Add More Courses\n";
        cout << "4. Remove Course\n";  // New option for removing courses
        cout << "5. Log Out\n";        // Adjusted menu
        setColor(15);
        cout << "Choose an option: ";
        studentChoice = v.inputNumber(5);

        switch (studentChoice) {
        case 1:
            setColor(15);  // White for details
            students[email].showDetails();
            cout << "\nPress any key to return to student menu...";
            _getch();
            break;

        case 2:
            setColor(15);  // White for course list
            cout << "\nYour Courses: \n";
            setColor(10);  // Light Green for course names
            for (const string& course : students[email].getCourses()) {
                cout << "- " << course << endl;
            }
            cout << "\nPress any key to return to student menu...";
            _getch();
            break;

        case 3: {
            setColor(14);  // Light Yellow for info
            if (students[email].getCourses().size() >= 3) {
                setColor(12);  // Red for error message
                cout << "You already have 3 courses. Please remove a course to add another.\n";
                cout << "\nPress any key to return to student menu...";
                _getch();
                break;
            }

            vector<string> newCourses = selectCourses(students[email].getCourses());
            students[email].renewCourse(newCourses);
            updateStudentCourseJson(students[email].getEmail(), newCourses);

            setColor(10);  // Light Green for success message
            cout << "Courses added successfully!" << endl;
            cout << "\nPress any key to return to student menu...";
            _getch();
            break;
        }

        case 4: {
            setColor(15);  // White for course removal
            cout << "Select a course to remove:\n";
            const auto& courses = students[email].getCourses();
            for (size_t i = 0; i < courses.size(); ++i) {
                cout << (i + 1) << ". " << courses[i] << endl;
            }
            cout << "Enter the number of the course to remove (or 0 to cancel): ";

            int courseChoice{};
            courseChoice = v.inputNumber(courses.size());

            if (courseChoice > 0 && courseChoice <= courses.size()) {
                string courseToRemove = courses[courseChoice - 1];
                students[email].removeCourse(courseToRemove);
                updateStudentCourseJson(students[email].getEmail(), students[email].getCourses());

                setColor(10);  // Light Green for success message
                cout << "Course \"" << courseToRemove << "\" removed successfully!" << endl;
            }
            else if (courseChoice == 0) {
                setColor(15);  // White for no action
                cout << "No course removed." << endl;
            }
            else {
                setColor(12);  // Red for invalid selection
                cout << "Invalid selection." << endl;
            }
            cout << "\nPress any key to return to student menu...";
            _getch();
            break;
        }

        case 5:
            setColor(15);  // White for logout info
            cout << "Logging out...\n";
            keepStudentLoggedIn = false;
            break;

        default:
            setColor(12);  // Red for invalid input
            cout << "Invalid option. Please try again.\n";
            Sleep(1000);
        }
    }
}

void adminMenu(string& email) {
    Validation v;
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
        cout << "1. View All Students\n2. View Domestic Students\n3. View International Students\n4. Remove a Student\n5. Remove a Student Course \n6. Search Student by Name\n7. Log Out\n";
        cout << "Choose an admin option: ";
        adminChoice = v.inputNumber(7);

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
            admins[email].viewAllStudentsNameandEmail(students);

            cout << "Enter student email to remove: ";
            v.discardExtraInput();
            getline(cin, studentEmail);
            while (true) {
                if (studentEmail == "0") break;
                if (students.find(studentEmail) == students.end()) {
                    cout << "Email is not found, please try again! Or enter 0 to cancel" << endl;
                    cout << "Email: ";
                    getline(cin, studentEmail);
                    continue;
                }
                break;
            }
            if (studentEmail == "0") break;

            admins[email].removeStudent(students, studentEmail);
            Sleep(1000);
            break;
        }
        case 5: {
            system("cls");
            admins[email].viewAllStudentsNameandEmail(students);

            string searchEmail{};
            cout << "Please enter the email of the student you want to remove the course of" << endl;
            cout << "Email : ";

            v.discardExtraInput();
            getline(cin, searchEmail);
            while (true) {
                if (searchEmail == "0") break;
                if (students.find(searchEmail) == students.end()) {
                    cout << "Email is not found, please try again! or enter 0 to cancel" << endl;
                    cout << "Email: ";
                    getline(cin, searchEmail);
                    continue;
                }
                break;
            }
            if (searchEmail == "0") break;

            admins[email].removeStudentCourse(students, searchEmail);

            cout << "\nPress any key to return to student menu...";
            _getch();
            break;
        }
        case 6: {
            system("cls");
            string nameToSearch{};
            cout << "Enter the name to search: ";
            cin >> nameToSearch;
            admins[email].searchStudentsByName(students, nameToSearch);
            cout << "\nPress any key to return to admin menu...";
            _getch();
            break;
        }
        case 7:
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
        "Bachelor of IT - Software Engineering", 
        "Diploma in Language Studies", 
        "Diploma in Management", 
        "Diploma in Film and Content Creation" 
    };

    vector<string> selectedCourses = studentCourses;
    int courseChoice{};

    if (!selectedCourses.empty()) {
        cout << "Your current course: " << endl;
        for (int i = 0; i < selectedCourses.size(); i++){
            cout << i + 1 << ". " << selectedCourses[i] << endl;
        }
        cout << endl;
    }
    

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

void loginProcedure() {
    Validation v;
    encrypt::Encryption encryptClass;
    string email, password;

    v.discardExtraInput();
    while (true) {
        cout << "Enter email: ";
        getline(cin, email);
        if (email == "0") {
            cout << "Returning to main menu..." << endl;
            return;
        }

        password = v.inputPassword();
        vector<uint8_t> encryptedPassword{};
        
        const uint8_t privateKey[16] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x97, 0x75, 0x46, 0x65, 0x1d, 0x37 };

        encryptClass.encryptPassword(password.c_str(), privateKey, encryptedPassword);
        password = encryptClass.symbolToHex(encryptedPassword);
        

        if (login(email, password)) {
            break;
        }

        cout << "Incorrect password or email, please enter again! or enter 0 to go back to main menu" << endl;
    }

    // Check if the user is a student and show their details
    if (students.find(email) != students.end()) {
        studentMenu(email);
        return;
    }
    // not really needed but makes life a whole lot easier, code more readable
    if (admins.find(email) != admins.end()) {
        adminMenu(email);
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
    ifstream inFile("adminnew.json");
    if (inFile.is_open()) {
        Admin::loadFromFileJSON(inFile, admins);
        inFile.close();
    }
}


// Sign-in function
bool login(const string& email, const string& password) {
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
