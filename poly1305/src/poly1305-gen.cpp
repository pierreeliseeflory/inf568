#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include "poly1305-core.hpp"

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        std::cout << "Usage : " << argv[0] << " key filename" << std::endl;
        return 1;
    }

    if (strlen(argv[1]) != 64) {
        std::cout << "The key must be 64 hexadecimal characters long." << std::endl;
        return 1;
    }

    unsigned char r[16];
    unsigned char s[16];
    unsigned char tag[16];

    std::string key(argv[1]);
    std::string hexa;

    for (int j = 0; j < 16; ++j) {
        hexa = key.substr(2*j, 2);
        r[j] = (unsigned char)std::stoi(hexa, 0, 16);
        hexa = key.substr(2*j + 32, 2);
        s[j] = (unsigned char)std::stoi(hexa, 0, 16);
    }

    std::ifstream file(argv[2]);
    std::stringstream buffer;
    buffer << file.rdbuf();

    unsigned int l = buffer.str().size();
    unsigned char *message = new unsigned char[l] ;


    for (unsigned int i = 0; i < l; ++i)
        message[i] = (unsigned char)buffer.str()[i];

    poly1305(r, s, message, l, tag);
    
    delete[] message;

    for (int j = 0; j < 16; ++j)  {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << (int)tag[j];
    }
    std::cout << std::endl;

    return 0;
}

// unsigned char r[16] = {0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33, 0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8};
// unsigned char s[16] = {0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd, 0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b};
// unsigned char message[34] = {'C', 'r', 'y', 'p', 't', 'o', 'g', 'r', 'a', 'p', 'h', 'i', 'c', ' ', 'F', 'o', 'r', 'u', 'm', ' ', 'R', 'e', 's', 'e', 'a', 'r', 'c', 'h', ' ', 'G', 'r', 'o', 'u', 'p'};
// unsigned int l = 34;

// test vectors
// 85d6be7857556d337f4452fe42d506a8
// 0103808afb0db2fd4abff6af4149f51b
// 85d6be7857556d337f4452fe42d506a80103808afb0db2fd4abff6af4149f51b
// Cryptographic Forum Research Group

/*
Input:
    * (r, s) // 32 bytes
    * message // arbitrary length

Output :
    * tag // 16 bytes

clamping r :
    from D. J. Bernstein

    void poly1305_clamp(unsigned char r[16]) {
        r[3] &= 15;
        r[7] &= 15;
        r[11] &= 15;
        r[15] &= 15;
        r[4] &= 252;
        r[8] &= 252;
        r[12] &= 252;
    }

Endianness : little-endian

Algorithm: (from RFC)
    Clamp r
    Field prime : P = 2^130 - 5 = 0x3fffffffffffffffffffffffffffffffb
    Accumulator (acc) set to 0

    M = (M_0, ..., M_n-1, M_n)
        M_i // 16 bytes
        M_n // <= 16 bytes

    [... + 2^(number of bits)]

    for i=1 upto ceil(msg length in bytes / 16)
        n = le_bytes_to_num(msg[((i-1)*16)..(i*16)] | [0x01])
        a += n
        a = (r * a) % p
        end
    a += s
    return num_to_16_le_bytes(a)
*/