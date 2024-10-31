#include "encrypt.h"

using namespace std;

string EncryptUtil::encrypt(string line) {
	int key = 0;
	for (int i = 0; i < line.length(); i++){
		key += int(line[i]);
	}

	string encrypted{};
	for (int i = 0; i < line.length(); i++) {
		encrypted += char(int(line[i]) + key);
	}

	return encrypted;
}

string EncryptUtil::decrypt(string encryptedLine, string password) {
	int key = 0;
	for (int i = 0; i < password.length(); i++) {
		key += int(password[i]);
	}

	string decrypted{};
	for (int i = 0; i < encryptedLine.length(); i++) {
		decrypted += char(int(encryptedLine[i]) - key);
	}

	return decrypted;
}


