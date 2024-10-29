#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <conio.h>  // For _getch() to hide password input on Windows
#include <sstream>
#include <windows.h>
#include <cstdlib> 

using namespace std;


void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void displayLogo() {
    setColor(3); 
    cout << "    ***************************************************************************************************************\n";
    setColor(9); 
    cout << "                    MMP\"\"MM\"\"YMM       7MM                     mm    7MMF    7MMF           7MM        \n";
    setColor(11); 
    cout << "                    P'   MM   `7        MM                     MM     MM      MM             MM         \n";
    setColor(14); 
    cout << "                         MM   ,6\"Yb.    MM  .gP\"Ya  7MMpMMMb.mmMMmm   MM      MM  7MM   7MM  MM,dMMb.  \n";
    setColor(10); 
    cout << "                         MM  8)   MM    MM ,M'   Yb  MM    MM  MM     MMmmmmmmMM   MM    MM  MM    `Mb \n";
    setColor(13); 
    cout << "                         MM   ,pm9MM    MM 8M\"\"\"\"\"\"  MM    MM  MM     MM      MM   MM    MM  MM     M8 \n";
    setColor(12); 
    cout << "                         MM  8M   MM    MM YM.    ,  MM    MM  MM     MM      MM   MM    MM  MM.   ,M9 \n";
    setColor(14); 
    cout << "                        JMML `Moo9^Yo  JMML `Mbmmd' JMML  JMML  Mbmo JMML    JMML  `Mbod\"YML.P^YbmdP'  \n";
    setColor(3);
    cout << "    ***************************************************************************************************************\n";
    setColor(15); 
}

void displayMenu() {
    setColor(11); // Cyan
    cout << "\n                           1. ";
    setColor(10); // Green
    cout << "Student Sign-Up";
    setColor(11); // Cyan
    cout << "\n                           2. ";
    setColor(14); // Yellow
    cout << "Sign In";
    setColor(11); // Cyan
    cout << "\n                           3. ";
    setColor(12); // Red
    cout << "Exit";
    setColor(15); // Reset to white
    cout << "\n                           Choose an option: ";
}


// Base User class
class User {
protected:
    string email;
    string password;
public:
    User() = default;
    User(string email, string password) : email(email), password(password) {}

    virtual bool login(string inputEmail, string inputPassword) {
        return (email == inputEmail && password == inputPassword);
    }

    string getEmail() const {
        return email;
    }
};

// Derived Student class
class Student : public User {
private:
    string firstName;
    string lastName;
    string course;
    bool isDomestic;
public:
    Student() = default;
    Student(string email, string password, string firstName, string lastName, string course, bool isDomestic)
        : User(email, password), firstName(firstName), lastName(lastName), course(course), isDomestic(isDomestic) {}

    bool getIsDomestic() const {
        return isDomestic;
    }

    void showDetails() const {
        cout << "Student Name: " << firstName << " " << lastName << endl;
        cout << "Email: " << email << endl;
        cout << "Course: " << course << endl;
        cout << "Status: " << (isDomestic ? "Domestic" : "International") << endl;
    }

    // Save student details to a file
    void saveToFile(ofstream& outFile) const {
        outFile << email << " " << password << " " << firstName << " " << lastName << " "
            << course << " " << isDomestic << endl;
    }

    // Load student details from a file
    static void loadFromFile(ifstream& inFile, unordered_map<string, Student>& students) {
        string email, password, firstName, lastName, course;
        bool isDomestic;
        while (inFile >> email >> password >> firstName >> lastName >> course >> isDomestic) {
            students[email] = Student(email, password, firstName, lastName, course, isDomestic);
        }
    }
};

// Derived Admin class
class Admin : public User {
public:
    Admin() = default;
    Admin(string email, string password) : User(email, password) {}

    void viewAllStudents(const unordered_map<string, Student>& students) const {
        cout << "\n-- All Students --" << endl;
        for (const auto& pair : students) {
            pair.second.showDetails();
            cout << "-----------------" << endl;
        }
    }

    void viewDomesticStudents(const unordered_map<string, Student>& students) const {
        cout << "\n-- Domestic Students --" << endl;
        for (const auto& pair : students) {
            if (pair.second.getIsDomestic()) {
                pair.second.showDetails();
                cout << "-----------------" << endl;
            }
        }
    }

    void viewInternationalStudents(const unordered_map<string, Student>& students) const {
        cout << "\n-- International Students --" << endl;
        for (const auto& pair : students) {
            if (!pair.second.getIsDomestic()) {
                pair.second.showDetails();
                cout << "-----------------" << endl;
            }
        }
    }

    void removeStudent(unordered_map<string, Student>& students, string studentEmail) const {
        if (students.find(studentEmail) != students.end()) {
            students.erase(studentEmail);
            cout << "Student with email " << studentEmail << " has been removed." << endl;
        }
        else {
            cout << "Student not found." << endl;
        }
    }

    // Load admin details from a file
    static void loadFromFile(ifstream& inFile, unordered_map<string, Admin>& admins) {
        string email, password;
        while (inFile >> email >> password) {
            admins[email] = Admin(email, password);
        }
    }
};

// Global storage for students and admins
unordered_map<string, Student> students;
unordered_map<string, Admin> admins;

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

// Main function
int main() {
    // Display TalentHub logo at start
    displayLogo();
    loadStudentsFromFile();
    loadAdminsFromFile();

    // Main menu loop for the system
    int choice;
    while (true) {
       
        displayMenu();
        cin >> choice;

        if (choice == 1) {
            setColor(10); // Green for success messages
            cout << "Student Sign-Up selected." << endl;
            setColor(15); // Reset color
            studentSignUp();
        }
        else if (choice == 2) {
            string email, password;
            cout << "Enter email: ";
            cin >> email;
            password = inputPassword();
            if (signIn(email, password)) { // can use better naming convention ableToSignIn()
                if (admins.find(email) != admins.end()) { // nested if statement is bad practice
                    setColor(10);
                    int adminChoice;
                    bool keepAdminLoggedIn = true;
                    while (keepAdminLoggedIn) { // this whole while loop can be put inside another function such as adminMenu()
                        cout << "\n-- Admin Menu --\n";
                        setColor(12);
                        cout << "1. View All Students\n2. View Domestic Students\n3. View International Students\n4. Remove a Student\n5. Sign Out\n";
                        cout << "Choose an admin option: ";
                        cin >> adminChoice;

                        switch (adminChoice) {
                        case 1:
                            admins[email].viewAllStudents(students);
                            break;
                        case 2:
                            admins[email].viewDomesticStudents(students);
                            break;
                        case 3:
                            admins[email].viewInternationalStudents(students);
                            break;
                        case 4: {
                            string studentEmail;
                            cout << "Enter student email to remove: ";
                            cin >> studentEmail;
                            admins[email].removeStudent(students, studentEmail);
                            break;
                        }
                        case 5:
                            keepAdminLoggedIn = false; // Exit admin menu
                            system("cls"); // Clear screen after sign-out
                            displayLogo();
                            break;
                        default:
                            cout << "Invalid option. Please try again." << endl;
                        }
                    }
                }
            }
        }
        else if (choice == 3) {
            break; // Exit program
        }
        else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}