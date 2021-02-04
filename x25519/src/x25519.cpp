#include "montgomery.hpp"
#include <iostream>
#include <iomanip>

/*
x25519(m, P) ->
*/

// @Bernstein
mpz_class clampingScalar(unsigned char scalar[32]) {
    scalar[0]  &= 248;
    scalar[31] &= 127;
    scalar[31] |= 64;

    mpz_class res;
    for (int i = 0; i < 32; ++i) {
        res += ((mpz_class)scalar[i] << 8*i);
    }
    return res;
}

mpz_class decodeUCoordinate(unsigned char coordinate[32]) {
    coordinate[31] &= 127;
    mpz_class res;
    for (int i = 0; i < 32; ++i)
        res += ((mpz_class)coordinate[i] << 8*i);
    return res;
}

void displayKey(mpz_class x) {
    for (int i = 0; i < 32; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << ((x >> 8 * i) & 0xff);
    std::cout << std::endl;
}

// void inputHexa(std::string s, std::string c, mpz_class &clampedScalar, mpz_class &decodedCoordinate) {
//     unsigned char scalar[32];
//     unsigned char coordinate[32];
//     std::string hexa;

//     for (int j = 0; j < 32; ++j) {
//         hexa = s.substr(2*j, 2);
//         scalar[j] = (unsigned char)std::stoi(hexa, 0, 16);
//         hexa = c.substr(2*j, 2);
//         coordinate[j] = (unsigned char)std::stoi(hexa, 0, 16);
//     }

//     clampedScalar = clampingScalar(scalar);
//     decodedCoordinate = decodeUCoordinate(coordinate);
// }

void inputHexa(std::string s, mpz_class &mpz, mpz_class (*f)(unsigned char*)) {
    unsigned char arary[32];
    std::string hexa;

    for (int j = 0; j < 32; ++j) {
        hexa = s.substr(2*j, 2);
        arary[j] = (unsigned char)std::stoi(hexa, 0, 16);
    }

    mpz = f(arary);
}

mpz_class x25519(mpz_class clampedScalar, mpz_class decodedCoordinate) {
    curve_point point;
    point.X = decodedCoordinate;
    point.Z = 1;

    mpz_class p = (((mpz_class) 1) << 255) - 19;
    // mpz_class A = 486662;
    mpz_class A24 = 121666;
    // mpz_class A24 = 121665;

    return montgomeryLadder(clampedScalar, point, A24, p);
}

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3) {
        std::cout << "usage: ./x25519 secret [public]" << std::endl;
        return 1;
    }

    std::string s(argv[1]);
    mpz_class clampedScalar;
    mpz_class decodedCoordinate;

    inputHexa(s, clampedScalar, clampingScalar);

    if (argc == 2) {
        decodedCoordinate = 9;
    } else {
        s = argv[2];
        inputHexa(s, decodedCoordinate, decodeUCoordinate);
    }
    
    mpz_class key = x25519(clampedScalar, decodedCoordinate);

    displayKey(key);

    return 0;
}