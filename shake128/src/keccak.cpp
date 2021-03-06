#include "keccak.hpp"

// performs the permutations according to the FIPS
void keccak_p (keccak_state &state) {
    for (int i_r = 0; i_r < 24; ++i_r) {
        theta(state);
        rho(state);
        pi(state);
        chi(state);
        iota(state, i_r);
    }
}

// performs the padding both for a regula Keccak (10*1 padding) and the SHAKE128 specific padding (1111)
std::vector<std::bitset<1344>> shake128Padding(std::string input) {
    int size_string = input.size();

    int padded_size = 1;
    while (8*size_string + 6 > padded_size*1344) padded_size++;

    std::vector<std::bitset<1344>> padded_message;
    std::bitset<1344> block;  

    for (int t = 0; t < padded_size; ++t) {
        padded_message.push_back(block);
    }

    for (int k = 0; k < size_string; ++k) {
        int bits = 8*k;
        int block_number = bits / 1344;
        bits %= 1344;

        for (int i = 0; i < 8; ++i) {
            padded_message[block_number][bits + i] = input[k] & (1 << i);
        }
    }
    int block_number = 8*size_string / 1344;
    for (int i = 0; i < 5; ++i) padded_message[block_number][((8*size_string )%1344)+ i] = 1;
    padded_message.back()[1344-1] = 1;

    return padded_message;
}

// absorption part of the sponge
keccak_state absorb(std::vector<std::bitset<1344>> padded_message) {
    keccak_state state;
    int number_absorb_round = padded_message.size();

    for (int i = 0; i < number_absorb_round; ++i) {
        for (int k = 0; k < 1344; ++k) {
            int y = k / 320;
            int x = (k%320)/64;
            int z = (k%64);
            state[x][y][z] = state[x][y][z] ^ padded_message[i][k];
        }

        keccak_p(state);
    }

    return state;
}

// squeezing part of the sponge
void squeeze (keccak_state & state, int output_size) {
    std::array<std::bitset<64>,1344/64> output;

    int number_squeeze_round = output_size / 1344;
    int remaining_bits = output_size % 1344;
    if (remaining_bits != 0) number_squeeze_round++;

    std::bitset<64> tmp;
    std::bitset<8> hex;

    std::cout << std::hex ;

    for (int squeeze_round = 1; squeeze_round < number_squeeze_round; ++squeeze_round) {
        for (int k = 0; k < 1344; ++k) {
            output[k/64][k%64] = state[(k%320)/64][k / 320][k%64];
        }
        
        for (int i = 0; i < 1344/64; ++i) {
            for (int j = 0; j < 4; ++j) {
                for (int k = 0; k < 8; ++k) {
                    tmp[8*(7-j)+k] = output[i][8*j + k];
                    tmp[8*j + k] = output[i][8*(7-j)+k];
                }
            }
            std::cout << std::setw(16) << std::setfill('0') << tmp.to_ulong();
        }
        keccak_p(state);
    }

    // last squeeze (no permutations and may be not perfectly aligned with r (or even 64))
    for (int k = 0; k < 1344; ++k) {
        output[k/64][k%64] = state[(k%320)/64][k / 320][k%64];
    }
    for (int i = 0; i < remaining_bits/64; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 8; ++k) {
                tmp[8*(7-j)+k] = output[i][8*j + k];
                tmp[8*j + k] = output[i][8*(7-j)+k];
            }
        }
        std::cout << std::setw(16) << std::setfill('0') << tmp.to_ulong();
    }
    // for output to be meaningful, we need the output_size to be a multiple of 4 (represented by 1 hex char)
    if (remaining_bits%64 != 0) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 8; ++k) {
                tmp[8*(7-j)+k] = output[remaining_bits/64][8*j + k];
                tmp[8*j + k] = output[remaining_bits/64][8*(7-j)+k];
            }
        }

        std::stringstream stream;
        stream << std::hex << tmp.to_ulong();
        std::string result( stream.str().substr(0, (remaining_bits%64/4)) );
        std::cout << result;
    }
    std::cout << std::endl;
}

// perform the sponge function
void sponge (std::string input, int output_size) {
    std::vector<std::bitset<1344>> padded_message = shake128Padding(input);

    keccak_state state = absorb(padded_message);
    squeeze(state, output_size);
}

// read the input, call the sponge
void shake128(int output_size) {
    std::string input("");
    char buf;
    int count = 0;
    while (fread(&buf, sizeof(char), 1, stdin) == 1) {
        ++count;
        input += buf;
    }
    sponge(input, output_size);
}

int main(int argc, char *argv[])
{  
    if (!(argc == 2)) {
        std::cout << "Usage: ./shake128 size_output" << std::endl;
        return 1;
    }
    // I initially used bits (by multiple of 4 for a correct output)
    shake128(8*atoi(argv[1]));

    return 0;
}