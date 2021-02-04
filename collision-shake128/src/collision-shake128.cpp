#include <iostream>
#include <iomanip>
#include "FIPS202-opt64/SimpleFIPS202.h"
#include <unordered_map>
#include <string>

const int outputByteLen = 6;

// const char characters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
// const char characters[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const char characters[] = {' ', '\t', '\n'};

void print(unsigned char *output) {
    for (size_t i = 0; i < outputByteLen; ++i)
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)output[i];
}

// void print(unsigned char *output) {
//     for (size_t i = 0; i < outputByteLen; ++i)
//         std::cout << output[i];
// }

bool equalHashes(const unsigned char *hash1, const unsigned char *hash2) {
    for (int i = 0; i < outputByteLen; ++i) {
        if (hash1[i] != hash2[i]) return false;
    }
    return true;
}

void bruteforce(){
    const int max_size = 16;
    int values[max_size] = { 0 };
    for (int i = 0; i < max_size; ++i) values[i] = -1;

    unsigned char output[outputByteLen];
    // int char_size = 62;
    int char_size = 3;

    std::unordered_map<std::string, std::string> hashes;
    std::stringstream ss;

    std::string hash;
    
    while (true) {
        size_t inputByteLen = 34;
        std::string prefix = "pierre-elisee.flory@ensta-paris.fr";
        std::string suffix = "";
        std::string complete = "";

        for (int i = 0; i < max_size; ++i) {
            if (values[i] == -1) break;
            suffix += characters[values[i]];
            inputByteLen++;
        }
        complete = prefix + suffix;

        const unsigned char *input = reinterpret_cast<const unsigned char *>(complete.c_str());
        SHAKE128(output, outputByteLen, input, inputByteLen);

        for(int i = 0; i < outputByteLen; ++i)
            ss << std::setfill('0') << std::setw(2) << std::hex << (int)output[i];
        hash = ss.str();
        ss.str(std::string());
        
        auto search = hashes.find(hash);
        if (search != hashes.end()) {
            std::cout << "-------" << std::endl;
            std::cout << prefix << suffix << std::endl;
            std::cout << "-" << std::endl;
            std::cout << prefix << hashes.at(hash) << std::endl;
        } else if ( hashes.size() < (1000000)) {
            hashes.insert({hash, suffix});
        }

        for (int i = 0; i < max_size; ++i) {
            if (++values[i] != char_size) break;
            values[i] = 0;
        }
    }

}

void rho(unsigned char *input, int inputByteLen){
    long int s = 1;

    unsigned char H[outputByteLen];
    unsigned char T[outputByteLen];

    SHAKE128(T, outputByteLen, input, inputByteLen);
    SHAKE128(H, outputByteLen, T, outputByteLen);

    while(!equalHashes(T, H)) {
        s++;
        SHAKE128(T, outputByteLen, T, outputByteLen);
        SHAKE128(H, outputByteLen, H, outputByteLen);
        SHAKE128(H, outputByteLen, H, outputByteLen);
    }

    std::cout << s << std::endl;

    unsigned char T1[outputByteLen];
    unsigned char T2[outputByteLen];

    for (int i = 0; i < outputByteLen; ++i) {
        T1[i] = T[i];
    }

    unsigned char T1_prime[outputByteLen];
    unsigned char T2_prime[outputByteLen];

    SHAKE128(T1_prime, outputByteLen, T1, outputByteLen);
    SHAKE128(T2_prime, outputByteLen, input, inputByteLen);

    while (!equalHashes(T1_prime, T2_prime)) {
        for (int i = 0; i < outputByteLen; ++i) {
            T1[i] = T1_prime[i];
            T2[i] = T2_prime[i];
        }
        SHAKE128(T1_prime, outputByteLen, T1, outputByteLen);
        SHAKE128(T2_prime, outputByteLen, T2, outputByteLen);
    }
    std::cout << "preimage n°1 : ";
    print(T1);
    std::cout << " preimage n°2 : ";
    print(T2);
    std::cout << " hash : ";
    print(T1_prime);
    std::cout << std::endl;
}

// void findCollisions(int numberBytes) {
//     int ctr = 0;
//     while (true) {
//     }
// }

int main()
{
    // bruteforce();

    unsigned char input[4] = "pe";

    rho(input, 2);

    return 0;
}
