#include "keccak.hpp"
#include "permutations.hpp"

void printState(keccak_state state) {
    std::cout << std::hex << std::uppercase << std::endl;
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            std::cout << std::setw(16) << std::setfill('0') << state[x][y].to_ulong() << " ";;
        }
        std::cout << "\n";
    }
        std::cout << std::endl;
}

void keccak_p_with_display () {
    keccak_state state;

    std::cout << "Initial State" << "\n";
    printState(state);

    for (int i_r = 0; i_r < 24; ++i_r) {
        std::cout << "Round " << i_r << " :" << "\n";
        theta(state);

        std::cout << "After Theta" << "\n";
        printState(state);

        rho(state);
        std::cout << "After Rho" << "\n";
        printState(state);

        pi(state);
        std::cout << "After Pi" << "\n";
        printState(state);

        chi(state);
        std::cout << "After Chi" << "\n";
        printState(state);

        iota(state, i_r);
        std::cout << "After Iota" << "\n";
        printState(state);
    }

    stateLinearOutput(state);
}

void keccak_p (keccak_state &state) {
    for (int i_r = 0; i_r < 24; ++i_r) {
        theta(state);
        rho(state);
        pi(state);
        chi(state);
        iota(state, i_r);
    }
}

void stateLinearOutput(keccak_state state) {
    std::cout << std::hex << std::uppercase << std::endl;

    std::bitset<8> hexa;

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            for (int t = 0; t < 8; ++t) {
                for (int k = 0; k < 8; ++k) {
                    hexa[k] = state[x][y][8*t + k];
                }
                std::cout << std::setw(2) << std::setfill('0') << hexa.to_ulong() << " ";;
            }
        }
    }
        std::cout << std::endl;
}

// pad(x, m) -> m + len(pad(x, m)) est un multiple de x
// ici x = r = 1344
// m = len(N)

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
    
    // for (int i = 0; i < padded_size; ++i) {
    //     std::cout << padded_message[i] << std::endl;
    // }

    return padded_message;
}

void sponge (std::string input, const size_t output_size) {
    std::vector<std::bitset<1344>> padded_message = shake128Padding(input);

    keccak_state state = absorb(padded_message);
    squeeze(state, output_size);
}

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

void squeeze (keccak_state & state, const size_t output_size) {
    std::array<std::bitset<64>,1344/64> output;

    int number_squeeze_round = output_size / 1344;
    int remaining_bits = output_size % 1344;
    if (remaining_bits != 0) number_squeeze_round++;

    std::cout << std::hex << std::uppercase;

    for (int i = 0; i < number_squeeze_round - 1; ++number_squeeze_round) {
        for (int k = 0; k < 1344; ++k) {
            output[k/64][k%64] = state[(k%320)/64][k / 320][k%64];
        }
        keccak_p(state);
        
        for (int i = 0; i < 1344/64; ++i) {
            std::cout << std::setw(16) << std::setfill('0') << output[i].to_ulong() << " ";;
        }
    }
    // if the output isn't a multiple of r
    for (int k = 0; k < 1344; ++k) {
        output[k/64][k%64] = state[(k%320)/64][k / 320][k%64];
    }
    for (int i = 0; i < remaining_bits/64; ++i) {
        std::cout << std::setw(16) << std::setfill('0') << output[i].to_ulong() << " ";;
    }
    if (remaining_bits%64 != 0) {
        for (int i = remaining_bits%64; i < 64; ++i) {
            output[remaining_bits/64][i] = 0;
        }
        std::cout << std::setw(16) << std::setfill('x') << output[remaining_bits/64].to_ulong() << " ";;
    }
}

int main(int argc, char const *argv[])
{  
    // keccak_state state;

    // keccak_p(state);

    std::string input("OK");
    const size_t d = 512;
    sponge(input, d);

    return 0;
}