#pragma once

#include <iostream>
#include <string>
#include <vector>

class EncryptUtil {
public:
	std::string encrypt(std::string line);
	std::string decrypt(std::string line, std::string password);
};