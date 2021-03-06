#include <gmpxx.h>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "poly1305-core.hpp"

int main(int argc, char const *argv[])
{
    if (argc < 4) {
        std::cout << "Usage : " << argv[0] << " key filename authenticator" << std::endl;
        return 1;
    }

    // unsigned char r[16] = {0x85, 0xd6, 0xbe, 0x78, 0x57, 0x55, 0x6d, 0x33, 0x7f, 0x44, 0x52, 0xfe, 0x42, 0xd5, 0x06, 0xa8};
    // unsigned char s[16] = {0x01, 0x03, 0x80, 0x8a, 0xfb, 0x0d, 0xb2, 0xfd, 0x4a, 0xbf, 0xf6, 0xaf, 0x41, 0x49, 0xf5, 0x1b};
    // unsigned char message[34] = {'C', 'r', 'y', 'p', 't', 'o', 'g', 'r', 'a', 'p', 'h', 'i', 'c', ' ', 'F', 'o', 'r', 'u', 'm', ' ', 'R', 'e', 's', 'e', 'a', 'r', 'c', 'h', ' ', 'G', 'r', 'o', 'u', 'p'};
    // unsigned int l = 34;

    if (strlen(argv[1]) != 64) {
        std::cout << "The key must be 64 hexadecimal characters long." << std::endl;
        return 1;
    }

    unsigned char r[16];
    unsigned char s[16];
    unsigned char tag[16];
    unsigned char authenticator[16];

    std::string key(argv[1]);
    std::string buff(argv[3]);
    std::string hexa;

    for (int j = 0; j < 16; ++j) {
        hexa = key.substr(2*j, 2);
        r[j] = (unsigned char)std::stoi(hexa, 0, 16);
        hexa = key.substr(2*j + 32, 2);
        s[j] = (unsigned char)std::stoi(hexa, 0, 16);
        hexa = buff.substr(2*j, 2);
        authenticator[j] = (unsigned char)std::stoi(hexa, 0, 16);
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

    for (int j = 0; j < 16; ++j) {
        if (tag[j] != authenticator[j]) {
            std::cout << "REJECT" << std::endl;
            return 1;
        }
    }
    std::cout << "ACCEPT" << std::endl;

    return 0;
}