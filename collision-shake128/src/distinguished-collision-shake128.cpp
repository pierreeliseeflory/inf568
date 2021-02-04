#include <iostream>
#include <iomanip>
#include "FIPS202-opt64/SimpleFIPS202.h"
#include <unordered_map>
#include <string>
#include <vector>

#include <mpi.h>

const int outputByteLen = 6;

const unsigned char prefix[35] = "pierre-elisee.flory@ensta-paris.fr";
const int size_prefix = 34;

bool equalHashes2(const unsigned char *hash1, const std::vector<unsigned char> hash2, int outputByteLen) {
    for (int i = 0; i < outputByteLen; ++i) {
        if (hash1[i] != hash2[i]) return false;
    }
    return true;
}

bool equalHashes(const unsigned char *hash1, const unsigned char *hash2, int outputByteLen) {
    for (int i = 0; i < outputByteLen; ++i) {
        if (hash1[i] != hash2[i]) return false;
    }
    return true;
}

void print(unsigned char *output, size_t outputByteLen) {
    for (size_t i = 0; i < outputByteLen; ++i)
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)output[i];
    std::cout << std::endl;
}

void printVector(std::vector<unsigned char> output, size_t outputByteLen) {
    for (size_t i = 0; i < outputByteLen; ++i)
        std::cout << std::setfill('0') << std::setw(2) << std::hex << (int)output[i];
    std::cout << std::endl;
}

int main() {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    char verstring[MPI_MAX_LIBRARY_VERSION_STRING];
    int version, subversion, verstringlen;

    MPI_Get_version(&version, &subversion);
    MPI_Get_library_version(verstring, &verstringlen);

    // Get the rank of the process
    int process_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);

    if (process_rank == 0)
    {
      printf("Version %d, subversion %d\n", version, subversion);
      printf("Library <%s>\n", verstring);
    }

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_Barrier(MPI_COMM_WORLD);

    // Print off a hello world message
    printf("  - process rank %d out of %d processors from node %s\n",
           process_rank, world_size, processor_name);

    MPI_Barrier(MPI_COMM_WORLD);

    unsigned char hash_value[outputByteLen];

    MPI_Status status;
    MPI_Request request;

    // Master that gather distinguished points and perform 
    if (process_rank == 0) {
        std::cout << "Beginning collision search" << std::endl;

        // std::vector<std::vector<unsigned char[outputByteLen]>> distinguished_values(world_size);
        std::vector<std::vector<std::vector<unsigned char>>> distinguished_values(world_size);
        std::vector<std::vector<int>> distinguished_trail(world_size);
        int sender;

        while (true) {
            MPI_Recv(hash_value, outputByteLen, MPI_UNSIGNED_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            sender = status.MPI_SOURCE;

            distinguished_values.at(sender).push_back({hash_value, hash_value + outputByteLen});
            distinguished_trail.at(sender).push_back(status.MPI_TAG);

            for (int process = 1; process < world_size; ++process) {
                if (process != sender) {
                    //check if in vector
                    for (size_t distinct = 0; distinct < distinguished_values.at(process).size(); ++distinct) {
                        // std::cout << distinct << " " << process << std::endl;
                        // printVector(distinguished_values.at(process).at(distinct), outputByteLen);
                        // std::cout << &distinguished_values.at(process).at(distinct) << std::endl;
                        // print(hash_value, outputByteLen);
                        // std::cout << &hash_value << std::endl;

                        if (equalHashes2(hash_value, distinguished_values.at(process).at(distinct), outputByteLen)) {
                            std::cout << "   Found collision ! Process : " << process << " and " << sender << std::endl;
                            std::cout << "   sender : " << status.MPI_SOURCE << std::endl;
                            std ::cout << "   ";
                            print(hash_value, outputByteLen);

                            // previous hash
                            if (distinct == 0) {
                                for (int i = 0; i < outputByteLen; ++i) hash_value[i] = (unsigned char)process;
                                MPI_Isend(hash_value, outputByteLen, MPI_UNSIGNED_CHAR, sender, distinguished_trail.at(process).at(distinct), MPI_COMM_WORLD, &request); 
                            } else {
                                // printVector(distinguished_values.at(process).at(distinct - 1), outputByteLen);
                                MPI_Isend(&distinguished_values.at(process).at(distinct - 1).front(), outputByteLen, MPI_UNSIGNED_CHAR, sender, distinguished_trail.at(process).at(distinct), MPI_COMM_WORLD, &request); 
                            }

                            // hash received
                            int previous_hash = distinguished_values.at(sender).size() - 2;
                            if (previous_hash == -1) {
                                for (int i = 0; i < outputByteLen; ++i) hash_value[i] = (unsigned char)sender;
                                MPI_Isend(hash_value, outputByteLen, MPI_UNSIGNED_CHAR, sender, distinguished_trail.at(process).at(distinct), MPI_COMM_WORLD, &request);
                            } else {
                                // printVector(distinguished_values.at(sender).at(previous_hash), outputByteLen);
                                MPI_Isend(&distinguished_values.at(sender).at(previous_hash).front(), outputByteLen, MPI_UNSIGNED_CHAR, sender, status.MPI_TAG, MPI_COMM_WORLD, &request);
                            }
                        }
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < outputByteLen; ++i) hash_value[i] = (unsigned char)process_rank;

        // slaves performing chain of hashes

        int flag = 0;
        int trail_length = 0;

        unsigned char input[outputByteLen + size_prefix];
        std::copy(prefix, prefix + size_prefix, input);

        while (!flag) {
            /* code */
            std::copy(hash_value, hash_value + outputByteLen, input + size_prefix);


            SHAKE128(hash_value, outputByteLen, input, outputByteLen + size_prefix);

            trail_length++;

            if (hash_value[0] == 0 && hash_value[1] == 0) {
                MPI_Isend(hash_value, outputByteLen, MPI_UNSIGNED_CHAR, 0, trail_length, MPI_COMM_WORLD, &request);
                trail_length = 0;
            }
            MPI_Iprobe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
        }
        std::cout << "Beginning trail back" << std::endl;

        unsigned char hash_value2[outputByteLen];
        int trail1, trail2;

        MPI_Recv(hash_value, outputByteLen, MPI_UNSIGNED_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        trail1 = status.MPI_TAG;
        MPI_Recv(hash_value2, outputByteLen, MPI_UNSIGNED_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        trail2 = status.MPI_TAG;
        
        while (trail1 > trail2) {
            std::copy(hash_value, hash_value + outputByteLen, input + size_prefix);

            SHAKE128(hash_value, outputByteLen, input, outputByteLen + size_prefix);
            trail1--;
        }
        while (trail2 > trail1) {
            std::copy(hash_value2, hash_value2 + outputByteLen, input + size_prefix);
            SHAKE128(hash_value2, outputByteLen, input, outputByteLen + size_prefix);
            trail2--;
        }

        unsigned char hash_value1prev[outputByteLen];
        unsigned char hash_value2prev[outputByteLen];
        
        while (!equalHashes(hash_value, hash_value2, outputByteLen)) {
            for (int i = 0; i < outputByteLen; ++i) {
                hash_value1prev[i] = hash_value[i];
                hash_value2prev[i] = hash_value2[i];
            }
            std::copy(hash_value1prev, hash_value1prev + outputByteLen, input + size_prefix);
            SHAKE128(hash_value, outputByteLen, input, outputByteLen + size_prefix);

            std::copy(hash_value2prev, hash_value2prev + outputByteLen, input + size_prefix);
            SHAKE128(hash_value2, outputByteLen, input, outputByteLen + size_prefix);
            trail1--;
            if (trail1 < 0) {
                std::cout << "il y a un probleme" << std::endl;
                break;
            }
        }
        std::cout << "preimage n°1 : ";

        std::copy(hash_value1prev, hash_value1prev + outputByteLen, input + size_prefix);

        print(input, outputByteLen + size_prefix);
        std::cout << "preimage n°2 : ";
        std::copy(hash_value2prev, hash_value2prev + outputByteLen, input + size_prefix);

        print(input, outputByteLen + size_prefix);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}