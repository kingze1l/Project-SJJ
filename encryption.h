#pragma once
#include "encryption/aes.hpp"
#include <vector>
#include <string>

namespace encrypt {
	class Encryption {
	public:
		std::string symbolToHex(const std::vector<uint8_t>& vec);
		std::string hexToSymbols(const std::string& hex);
		void encryptPassword(const char* password, const uint8_t* key, std::vector<uint8_t>& encrypted);
		void decryptPassword(const std::vector<uint8_t>& encrypted, const uint8_t* key, std::vector<uint8_t>& decrypted);
	};
}
