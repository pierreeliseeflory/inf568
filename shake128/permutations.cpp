#include "permutations.hpp"

// 1600 bits 5 * 5 * 64

// b = 1600
// n_r = 24
// w = 64
// l = 6
// r = 1344
// c = 256

// 0 <= x < 5
// 0 <= y < 5
// 0 <= z < w (64)

unsigned modulo( int value, unsigned m) {
    int mod = value % (int)m;
    if (mod < 0) {
        mod += m;
    }
    return mod;
}

void theta(keccak_state &state) {
    keccak_column C, D;

    for (int x = 0; x < 5; ++x) {
        for (int z = 0; z < 64; ++z)
            C[x][z] = state[x][0][z] ^ state[x][1][z] ^ state[x][2][z] ^ state[x][3][z] ^ state[x][4][z];
    }

    for (int x = 0; x < 5; ++x) {
        for (int z = 0; z < 64; ++z)
            D[x][z] = C[modulo(x-1, 5)][z] ^ C[modulo(x+1, 5)][modulo(z-1, 64)];
    }

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            for (int z = 0; z < 64; ++z) {
                state[x][y][z] = state[x][y][z] ^ D[x][z];
            }
        }
    }
}

void rho(keccak_state &state) {
    int x = 1;
    int y = 0;
    int tmp;

    keccak_state new_state;
    
    for (int t = 0; t < 24; ++t) {
        for (int z = 0; z < 64; ++z)
            new_state[x][y][z] = state[x][y][modulo(z - (t + 1)*(t + 2) / 2, 64)];
        
        tmp = x;
        x = y;
        y = modulo(2*tmp + 3*y, 5);
    }

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            if (x != 0 || y != 0) state[x][y] = new_state[x][y];
        }
    }    
}

void pi(keccak_state &state) {
    keccak_state new_state;

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            for (int z = 0; z < 64; ++z) {
                new_state[x][y][z] = state[modulo(x + 3*y, 5)][x][z];
            }
        }
    }

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            state[x][y] = new_state[x][y];
        }
    }    
}

void chi(keccak_state &state) {
    keccak_state new_state;

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            for (int z = 0; z < 64; ++z) {
                new_state[x][y][z] = state[x][y][z] ^ ((state[modulo(x + 1, 5)][y][z] ^ 1) & state[modulo(x + 2, 5)][y][z]);
            }
        }
    }

    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            state[x][y] = new_state[x][y];
        }
    }
}

bool rc(int t) {
    std::bitset<8> R(1);
    std::bitset<9> R2(0);

    for (unsigned int i = 1; i <= modulo(t, 255); ++i) {
        R2.reset();
        for (int k = 0; k < 8; ++k) R2.set(k+1, R[k]);
        R2[0] = R2[0] ^ R2[8];
        R2[4] = R2[4] ^ R2[8];
        R2[5] = R2[5] ^ R2[8];
        R2[6] = R2[6] ^ R2[8];
        for (int k = 0; k < 8; ++k) R.set(k, R2[k]);
    }
    return R[0];
}

void iota(keccak_state &state, int i_r) {
    std::bitset<64> RC(0);

    for (int j = 0; j <= 6; ++j) RC[(1 << j) - 1] = rc(j + 7*i_r);

    for (int z = 0; z < 64; ++z) {
        state[0][0][z] = state[0][0][z] ^ RC[z];
    }
}
