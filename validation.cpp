#include <string>
#include <conio.h>
#include <regex>
#include <string>
#include "validation.h"

using namespace std;

void Validation::discardExtraInput() {
    cin.clear();
    cin.ignore(1000, '\n');
}

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
bool Validation::isValidNZMobileNumber(const string& number) {
    regex pattern(R"(^02\d{1}\d{7}$|^02\d{1}[-\s]?\d{3}[-\s]?\d{4}$)");
    return regex_match(number, pattern);
}

int Validation::inputNumber(const int& numOfOption) {
    int num{};
    cin >> num;
    while (cin.fail() || (num > numOfOption)) {
        discardExtraInput();
        cout << "Please input from the options available!\nOption: ";
        cin >> num;
    }
    return num;
}

string Validation::inputPassword() {
    string password;
    char ch;
    cout << "Password: ";
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
    string email{};
    discardExtraInput();
    while (true) {
        cout << "Email: ";
        getline(cin, email);

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
            cout << "- No space" << endl;
        }
    }
    return password;
}

int Validation::inputAgeValidation() {
    int age{};
    cin >> age;
    while (true) {
        if (age >= 18 && age <=100 ) {
            return age;
        }

        if (cin.fail()) {
            discardExtraInput();
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
    getline(cin, address);

    return address;
}