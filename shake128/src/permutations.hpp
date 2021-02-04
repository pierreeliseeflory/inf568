#ifndef __PERMUTATIONS_H__
#define __PERMUTATIONS_H__

#include <iostream>
#include <cstdint>
#include <bitset>
#include <array>
#include <iomanip>
#include <cmath>

// a data type enabling to compute permutations on a structure similar to the one proposed in the FIPS
typedef std::array<std::array<std::bitset<64>, 5>, 5> keccak_state;

typedef std::array<std::bitset<64>, 5> keccak_plane;

unsigned modulo( int value, unsigned m);

void theta(keccak_state &);

void rho(keccak_state &);

void pi(keccak_state &);

void chi(keccak_state &);

bool rc(int);

void iota(keccak_state &, int);

#endif // __PERMUTATIONS_H__