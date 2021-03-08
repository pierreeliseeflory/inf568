#include <gmpxx.h>
#include <stdint.h>
/*
D. J. Bernstein
Public domain.
*/
void poly1305_clamp(unsigned char r[16]) {
    r[3] &= 15;
    r[7] &= 15;
    r[11] &= 15;
    r[15] &= 15;
    r[4] &= 252;
    r[8] &= 252;
    r[12] &= 252;
}


void poly1305(unsigned char r[16], unsigned char s[16], unsigned char *message, unsigned int l, unsigned char tag[16]) {
    mpz_class r_, s_, p, acc, n;

    poly1305_clamp(r);


    for (int j = 0; j < 16; ++j) {
        r_ += ((mpz_class) r[j]) << 8 * j;
        s_ += ((mpz_class) s[j]) << 8 * j;
    }

    p = (((mpz_class) 1) << 130) - 5;
    acc = 0;
    int q = l / 16;
    int k = l % 16;

    for (int i = 1; i <= q; ++i) {
        n = 0;
        for (int j = 0; j < 16; ++j) {
            n +=  ((mpz_class) message[16 * ( i - 1 ) + j]) << (8*j);
        }
        n += ((mpz_class) 1) << 128;


        acc += n;
        acc = (r_ * acc) % p;
    }
    if (k != 0) {
        n = 0;
        for (int j = 0; j < (k); ++j) {
            n +=  ((mpz_class) message[16 * q + j]) << (8*j);
        }
        n += ((mpz_class) 1) << (8*k);
        acc += n;
        acc = (r_ * acc) % p;
    }
    acc += s_;

    for (int j = 0; j < 16; ++j)  {
        n = acc%256;
        tag[j] = n.get_ui();
        acc >>= 8;
    }
    
    return;
}

void poly1305_chacha_key(uint8_t key[64], uint8_t message[], unsigned int message_size, uint8_t tag[16]) {
    uint8_t r[16];
    uint8_t s[16];
    for (int i = 0; i < 16; ++i) {
        r[i] = key[i];
        s[i] = key[16 + i];
    }
    poly1305(r, s, message, message_size, tag);
}