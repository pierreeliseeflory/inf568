#include "aead_chacha20_poly1305-core.hpp"
#include "poly1305-core.hpp"

void poly1305_key_gen(key key, nonce nonce, keystream out) {
    counter counter = 0;
    chacha20_block(key, nonce, counter, out); 

}

void generateAEAD(key key, nonce nonce, uint8_t plaintext[], uint64_t message_size, uint8_t AAD[], uint64_t AAD_size, uint8_t*& ciphertext, uint8_t*& AEAD, uint64_t &AEAD_size,uint8_t tag[]) {
    keystream poly1305_key;
    poly1305_key_gen(key, nonce, poly1305_key);

    // chacha
    ciphertext = new uint8_t[message_size];

    chacha20_encrypt(key, 1, nonce, plaintext, message_size, ciphertext);

    // poly1305
    int padding1 = (AAD_size % 16 == 0) ? 0 : 16 - (AAD_size % 16);
    int padding2 = (message_size % 16 == 0) ? 0 : 16 - (message_size % 16);

    AEAD_size = AAD_size + padding1 + message_size + padding2 + 16;

    AEAD = new uint8_t[AEAD_size];

    int byte = 0;
    for (uint64_t i = 0; i < AAD_size; ++i)
        AEAD[byte++] = AAD[i];
    for (int i = 0; i < padding1; ++i)
        AEAD[byte++] = 0;
    for (uint64_t i = 0; i < message_size; ++i)
        AEAD[byte++] = ciphertext[i];
    for (int i = 0; i < padding2; ++i)
        AEAD[byte++] = 0;
    for (int i = 0; i < 8; ++i) {
        AEAD[byte++] = AAD_size & 0xff;
        AAD_size = AAD_size >> 8;
    }
    uint64_t tmp = message_size;
    for (int i = 0; i < 8; ++i) {
        AEAD[byte++] = tmp & 0xff;
        tmp = tmp >> 8;
    }

    poly1305_chacha_key(poly1305_key, AEAD, AEAD_size, tag);
}
