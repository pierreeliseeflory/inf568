#include "utils.hpp"

// only required for debugging
void printState(keccak_state state) {
    std::cout << std::hex  << std::endl;
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            std::cout << std::setw(16) << std::setfill('0') << state[x][y].to_ulong() << " ";;
        }
        std::cout << "\n";
    }
        std::cout << std::endl;
}

// debugging version of the keccak rounds
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


// to compare with the example given
void stateLinearOutput(keccak_state state) {
    std::cout << std::hex  << std::endl;

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