#include <string>
#include <cstring>

#include "poly1305-core.hpp"
#include "chacha20-core.hpp"
#include "aead_chacha20_poly1305-core.hpp"

int main(int argc, char const *argv[])
{
     if (argc < 3 || argc > 4) {
        std::cout << "Usage : " << argv[0] << " key nonce" << std::endl;
        return 1;
    }

    if (strlen(argv[1]) != 64) {
        std::cout << "The key must be 64 hexadecimal characters long." << std::endl;
        return 1;
    }

    if (strlen(argv[2]) != 24) {
        std::cout << "The nonce must be 64 hexadecimal characters long." << std::endl;
        return 1;
    }

    std::string line;
    std::getline(std::cin, line);
    uint64_t plaintext_size = line.size();

    uint8_t *plaintext = new uint8_t[plaintext_size];

    for (unsigned int i = 0; i < plaintext_size; ++i) {
        plaintext[i] = (uint8_t)line[i];
    }

    key key;
    nonce nonce;
   
    uint8_t *ciphertext;
    uint8_t *AEAD;
    uint64_t AEAD_size;
    uint8_t tag[16];

    std::string keyString(argv[1]);
    std::string nonceString(argv[2]);
    std::string hexa;

    for (int j = 0; j < 32; ++j) {
        hexa = keyString.substr(2*j, 2);
        key[j] = (uint8_t)std::stoi(hexa, 0, 16);
    }

    for (int j = 0; j < 3; ++j) {
        uint32_t tmp = 0;
        for (int i = 0; i < 4; ++i) {
            hexa = nonceString.substr(2*(4*j + i), 2);
            tmp += (uint32_t)std::stoi(hexa, 0, 16) << 8*i;
        }
        nonce[j] = tmp;
    }

    uint8_t *AAD = { 0x0 };
    uint64_t AAD_size = 0;

    if (argc == 4) {
        AAD_size = strlen(argv[3]) / 2;
        AAD = new uint8_t[AAD_size];
        std::string AADString(argv[3]);
        for (unsigned int j = 0; j < AAD_size; ++j) {
            hexa = AADString.substr(2*j, 2);
            AAD[j] = (uint8_t)std::stoi(hexa, 0, 16);
        }
    }

    generateAEAD(key, nonce, plaintext, plaintext_size, AAD, AAD_size, ciphertext, AEAD, AEAD_size, tag);

    std::cout << "Ciphertext:" << std::endl;
    for (unsigned int i = 0; i < plaintext_size; ++i) {
        std::cout << ciphertext[i];
    }
    std::cout << std::endl;

    std::cout << "AEAD:" << std::endl;
    for (unsigned int i = 0; i < AEAD_size; ++i) {
        std::cout << AEAD[i];
    }
    std::cout << std::endl;

    std::cout << "Tag:" << std::endl;
    for (int i = 0; i < 16; ++i) {
        std::cout << tag[i];
    }
    std::cout << std::endl;

    delete [] plaintext;
    delete [] ciphertext;
    delete [] AEAD;
    delete [] AAD;

    return 0;
}