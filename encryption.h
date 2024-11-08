#pragma once
#include "encryption/aes.hpp"
#include <vector>

namespace encrypt {
	class Encryption {
		void encryptPassword(const char* password, const uint8_t* key, std::vector<uint8_t>& encrypted);
		void decryptPassword(const std::vector<uint8_t>& encrypted, const uint8_t* key, std::vector<uint8_t>& decrypted);
	};
}
