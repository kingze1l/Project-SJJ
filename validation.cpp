#include <string>
#include <conio.h>
#include <regex>
#include "validation.h"

using namespace std;

bool Validation::isValidPassword(const string& password) {
    // regex pattern for password validation
    regex pattern("(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#$%^&*()-_+=])(?=\\S+$).{8,}");
    // checking password mattches the pattern 
    return regex_match(password, pattern);
}

bool Validation::isValidEmail(const string& email) {
    regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
}

int Validation::inputNumber(const int& numOfOption) {
    int num{};
    cin >> num;
    while (cin.fail() || (num > numOfOption)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please input from the options available!\nOption: ";
        cin >> num;
    }
    return num;
}

string Validation::inputPassword() {
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


string Validation::inputEmailValidation() {
    string email;
    while (true) {
        cout << "Email: ";
        cin >> email;

        if (email == "0") {
            return "";
        }

        if (!isValidEmail(email)) {
            cout << "Invalid email format. Please try again." << endl;
            continue;
        }

        if (students.find(email) == students.end()) {
            break;  // If email is unique, break the loop
        }
        else {
            cout << "Email already exists. Please enter a new different email. Or enter 0 to go back to menu" << endl;
        }
    }
    return email;
}

string Validation::inputPasswordValidation() {
    string password{};
    while (true) {
        password = inputPassword();
        if (isValidPassword(password)) {
            break;
        }
        else {
            cout << "Password is invalid. Please ensure it meets the following criteria:" << endl;
            cout << "- At least 8 characters long" << endl;
            cout << "- Contains at least one uppercase letter" << endl;
            cout << "- Contains at least one lowercase letter" << endl;
            cout << "- Contains at least one digit" << endl;
            cout << "- Contains at least one special character (e.g., @, #, $, etc.)" << endl;
        }
    }
    return password;
}

int Validation::inputAgeValidation() {
    int age{};
    cin >> age;
    while (true) {
        if (age >= 18) {
            return age;
        }

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Please input a number! " << endl;
            cout << "Age: ";
            cin >> age;
            continue;
        }

        cout << "This course is only available for those over 17, did you input the wrong number?" << endl;
        cout << "Age: ";
        cin >> age;
    }
}


string Validation::inputAddress() {
    string address{};
    cout << "Address: ";
    cin.clear();
    cin.ignore();
    getline(cin, address);

    return address;
}