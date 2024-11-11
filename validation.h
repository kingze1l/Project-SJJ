#pragma once
#ifndef USERVALIDATION_H
#define USERVALIDATION_H

#include <iostream>
#include <unordered_map>

#include "Class.h"

extern std::unordered_map<std::string, Student> students;
extern std::unordered_map<std::string, Admin> admins;

class Validation{
private:
	bool isValidPassword(const std::string& password);
	bool isValidEmail(const std::string& email);

public:
	/*std::string inputCourseString*/
	int inputNumber(const int& numOfOption);
	std::string inputPassword();
	std::string inputEmailValidation();
	std::string inputPasswordValidation();
	int inputAgeValidation();
	std::string inputAddress();
};

#endif // !USERVALIDATION_H

