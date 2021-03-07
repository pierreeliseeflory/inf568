#include "chacha20-core.hpp"

typedef uint32_t chacha_state[16];
typedef uint8_t key[32];
typedef uint32_t nonce[3];
typedef uint32_t counter;
typedef uint8_t keystream[64];

void printState(chacha_state state) {
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            std::cout << "0x";
            std::cout << std::hex << std::setfill('0') << std::setw(8) << state[column + row * 4] << " ";
        }
        std::cout << std::endl;
    }
}

void QUARTERROUND(chacha_state state, int ind0, int ind1, int ind2, int ind3) {
    uint32_t a = state[ind0];
    uint32_t b = state[ind1];
    uint32_t c = state[ind2];
    uint32_t d = state[ind3];
    
    a += b; d ^= a; d = (d << 16) | (d >> (32 - 16)); // d <<<= 16;
    c += d; b ^= c; b = (b << 12) | (b >> (32 - 12)); // b <<<= 12;
    a += b; d ^= a; d = (d <<  8) | (d >> (32 - 8)); // d <<<= 8;
    c += d; b ^= c; b = (b <<  7) | b >> (32 - 7); // b <<<= 7;

    state[ind0] = a;
    state[ind1] = b;
    state[ind2] = c;
    state[ind3] = d;
}

void inner_block(chacha_state state) {
    QUARTERROUND(state, 0, 4, 8, 12);
    QUARTERROUND(state, 1, 5, 9, 13);
    QUARTERROUND(state, 2, 6, 10, 14);
    QUARTERROUND(state, 3, 7, 11, 15);

    QUARTERROUND(state, 0, 5, 10, 15);
    QUARTERROUND(state, 1, 6, 11, 12);
    QUARTERROUND(state, 2, 7, 8, 13);
    QUARTERROUND(state, 3, 4, 9, 14);
    /*
    #pragma omp parallel num_threads(4) shared(state)
    {
        int i = omp_get_thread_num();
        if (i == 0) QUARTERROUND(state, 0, 4, 8, 12);
        if (i == 1) QUARTERROUND(state, 1, 5, 9, 13);
        if (i == 2) QUARTERROUND(state, 2, 6, 10, 14);
        if (i == 3) QUARTERROUND(state, 3, 7, 11, 15);
    }

    #pragma omp parallel num_threads(4) shared(state)
    {
        int i = omp_get_thread_num();
        if (i == 0) QUARTERROUND(state, 0, 5, 10, 15);
        if (i == 1) QUARTERROUND(state, 1, 6, 11, 12);
        if (i == 2) QUARTERROUND(state, 2, 7, 8, 13);
        if (i == 3) QUARTERROUND(state, 3, 4, 9, 14);
    }*/
}

void chacha20_block(key key, nonce nonce, counter counter, keystream out) {
    chacha_state state, initial_state;
    initial_state[0] = state[0] = 0x61707865;
    initial_state[1] = state[1] = 0x3320646e;
    initial_state[2] = state[2] = 0x79622d32;
    initial_state[3] = state[3] = 0x6b206574;

    int key_size = 32;
    int i;
    int nonce_size = 3;
    int number_rounds = 10;
    int size_state = sizeof(state) / sizeof(state[0]);

    for (int element = 0; element < key_size / 4; ++element) {
        uint32_t byte = 0;
        for (i = 0; i < 4; ++i) {
            byte += (key[4*element + i] << (8*i));
        }
        state[4 + element] = byte;
        initial_state[4 + element] = byte;
    }

    state[12] = counter;
    initial_state[12] = counter;

    for (i = 0; i < nonce_size; ++i) {
        state[13 + i] = nonce[i];
        initial_state[13 + i] = nonce[i];
    }

    for (int round = 0; round < number_rounds; ++round) {
        inner_block(state);
    }

    for (int element = 0; element < size_state; ++element) {
        state[element] += initial_state[element];
    }

    for (int element = 0; element < 16; ++element) {
        for (int byte = 0; byte < 4; ++byte) {
            out[4*element + byte] = (uint8_t)(state[element] >> (byte * 8));
        }
    }
}

void chacha20_encrypt(key key, counter counter, nonce nonce, uint8_t message[], int message_size, uint8_t cipher[]) {
    int block_number;
    keystream keystream;
    for (block_number = 0; block_number < (message_size / 64); ++block_number) {
        chacha20_block(key, nonce, counter++, keystream);
        for (int byte = 0; byte < 64; ++byte) {
            cipher[block_number * 64 + byte] = message[block_number * 64 + byte] ^ keystream[byte];
        }
    }
    if ((message_size % 64) != 0) {
        chacha20_block(key, nonce, counter, keystream);
        for (int byte = 0; byte < (message_size % 64); ++byte) {
            cipher[block_number * 64 + byte] = message[block_number * 64 + byte] ^ keystream[byte];
        }
    }
}