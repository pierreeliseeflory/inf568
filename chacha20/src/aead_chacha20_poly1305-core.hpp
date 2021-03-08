#ifndef __AEAD_CHACHA20_POLY1305_H__
#define __AEAD_CHACHA20_POLY1305_H__

#include "chacha20-core.hpp"

void poly1305_key_gen(key key, nonce nonce, keystream out);

void generateAEAD(key key, nonce nonce, uint8_t plaintext[], uint64_t message_size, uint8_t AAD[], uint64_t AAD_size, uint8_t*& ciphertext, uint8_t*& AEAD, uint64_t &AEAD_size, uint8_t tag[]);

#endif // __AEAD_CHACHA20_POLY1305_H__