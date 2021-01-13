#ifndef __KECCAK_H__
#define __KECCAK_H__

#include <bitset>
#include <array>
#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <string>

// a data type enabling to compute permutations on a structure similar to the one proposed in the FIPS
typedef std::array<std::array<std::bitset<64>, 5>, 5> keccak_state;

typedef std::array<std::bitset<64>, 5> keccak_column;

void keccak_p ();

void stateLinearOutput(keccak_state);

std::vector<std::bitset<1344>> shake128Padding(std::string);

void sponge (std::string, int);

keccak_state absorb(std::vector<std::bitset<1344>>);

void squeeze (keccak_state &, int);

void shake128(int);

#endif // __KECCAK_H__