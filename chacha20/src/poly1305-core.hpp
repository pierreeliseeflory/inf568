#ifndef __POLY1305_CORE_H__
#define __POLY1305_CORE_H__

#include <stdint.h>

void poly1305_clamp(unsigned char [16]);

void poly1305(unsigned char [16], unsigned char [16], unsigned char *, unsigned int, unsigned char [16]);
    
void poly1305_chacha_key(uint8_t key[64], uint8_t message[], unsigned int message_size, uint8_t tag[16]);

#endif // __POLY1305_CORE_H__