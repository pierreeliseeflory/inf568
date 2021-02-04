#ifndef __KECCAK_H__
#define __KECCAK_H__

#include <bitset>
#include <array>
#include <vector>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <string>

#include "permutations.hpp"
#include "utils.hpp"

void keccak_p ();

std::vector<std::bitset<1344>> shake128Padding(std::string);

void sponge (std::string, int);

keccak_state absorb(std::vector<std::bitset<1344>>);

void squeeze (keccak_state &, int);

void shake128(int);

#endif // __KECCAK_H__