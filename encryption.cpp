#include "encryption.h"

using namespace std;
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

void decryptPassword(const vector<uint8_t>& encrypted, const uint8_t* key, vector<uint8_t>& decrypted) {
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