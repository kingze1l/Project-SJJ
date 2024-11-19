#include "encryption.h"
#include <sstream>
#include <iomanip>

using namespace std;

string encrypt::Encryption::symbolToHex(const std::vector<uint8_t>& vec) {
    ostringstream oss{};
    for (uint8_t byte : vec) {
        oss << setw(2) << setfill('0') << hex << (int)byte;
    }
    return oss.str();
}


string encrypt::Encryption::hexToSymbols(const string& hex) {
    string res{};
    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byteString, nullptr, 16)); // convert hex to char
        res += byte;
    }
    return res;
}

void encrypt::Encryption::encryptPassword(const char* password, const uint8_t* key, std::vector<uint8_t>& encrypted) {
    AES_ctx ctx;
    AES_init_ctx(&ctx, key);

    size_t passwordLength = strlen(password);
    size_t numBlocks = (passwordLength + 15) / 16;

    encrypted.resize(numBlocks * 16, 0);

    for (size_t i = 0; i < numBlocks; i++) {
        size_t blockSize = min(size_t(16), passwordLength - (i * 16)); // Get size of the current block
        uint8_t padded[16] = { 0 }; // Buffer for this block

        // Copy the block into padded (with potential padding to 16 bytes)
        memcpy(padded, password + (i * 16), blockSize);


        AES_ECB_encrypt(&ctx, padded);
        memcpy(encrypted.data() + (i * 16), padded, 16);
    }
}

void encrypt::Encryption::decryptPassword(const vector<uint8_t>& encrypted, const uint8_t* key, vector<uint8_t>& decrypted) {
    AES_ctx ctx;
    AES_init_ctx(&ctx, key);

    size_t numBlocks = encrypted.size() / 16;
    decrypted.resize(numBlocks * 16, 0);

    for (size_t i = 0; i < numBlocks; i++) {
        uint8_t padded[16] = { 0 };
        memcpy(padded, encrypted.data() + (i * 16), 16);


        AES_ECB_decrypt(&ctx, padded);
        memcpy(decrypted.data() + (i * 16), padded, 16);
    }
}