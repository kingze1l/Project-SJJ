// CheckPasswordAndEncrypt.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>

#include "encrypt.h"

using namespace std;

int main()
{
	EncryptUtil encryptUtils;
	
	/*cout << "Please input a word that you want to encrypt : ";*/
	
	string password = "password980";
	string encryptedPassword = encryptUtils.encrypt(password);
	cout << encryptedPassword << endl;

	vector<string> ss{};
	for (int i = 0; i < 256; i++){
		string s{};
		for (int j = 0; j < encryptedPassword.length(); j++) {
			s += (int(encryptedPassword[j]) + i);
		}
		ss.push_back(s);
	}
	
	for (const auto i : ss){
		cout << i << "   ";
	}


	/*string decryptedPassword = encryptUtils.decrypt(encryptedPassword, password);
	cout << decryptedPassword;*/

}


// 13 characters and above, special character, uppercase

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
