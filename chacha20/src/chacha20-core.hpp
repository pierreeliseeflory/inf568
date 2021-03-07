#ifndef __CHACHA20_H__
#define __CHACHA20_H__

#include <stdint.h>
#include <omp.h>
#include <iomanip>
#include <iostream>

typedef uint32_t chacha_state[16];
typedef uint8_t key[32];
typedef uint32_t nonce[3];
typedef uint32_t counter;
typedef uint8_t keystream[64];

void printState(chacha_state state);

void QUARTERROUND(chacha_state state, int ind0, int ind1, int ind2, int ind3);

void inner_block(chacha_state state);

void chacha20_block(key key, nonce nonce, counter counter, keystream out) ;

void chacha20_encrypt(key key, counter counter, nonce nonce, uint8_t message[], int message_size, uint8_t cipher[]);

#endif // __CHACHA20_H__