#include "montgomery.hpp"
#include "ecm.hpp"

#include <iostream>
#include <utility>
#include <cmath>
#include <cstdlib>

// gcd(mpz_class, mpz_class)
// mpz_probab_prime_p (const mpz_t n, int reps), 15 < reps < 50 

void eratostheneSieve(int N) {
    std::vector<int> primes;

    primes.push_back(2);
    primes.push_back(3);
    for (int i = 5; i <= N; i+=2) {
        if (i%3 != 0)
            primes.push_back(i);
    }
    unsigned j = 0;
    int comp;

    while (j < primes.size() - 1) {
        if (primes[j] != 0) {
            comp = primes[j];

            for (unsigned i = j + 1; i < primes.size(); ++i) {
                if (primes[i] != 0) {
                    if ((primes[i]%comp) == 0)
                        primes[i] = 0;
                }
            }
        }
        j++;
    }

    int count = 0;

    for (auto element : primes) {
        if (element != 0) {
            count++;
            std::cout << element << ", ";
        }
    }
    std::cout << std::endl;
    std::cout << count << std::endl;
}

bool trial_division(mpz_class candidat, std::vector<std::pair<int, int>> &listFactors) {
    bool foundFactors = false;
    int exp;
    mpz_class test;
    for (auto prime : primes) {
        if (candidat % prime == 0) {
            foundFactors = true;
            exp = 1;
            test = prime*prime;
            while (candidat % test == 0) {
                exp++;
                test *= prime;
            }
            candidat /= (test/prime);
            listFactors.push_back({prime, exp});
        }
    }
    return foundFactors;
}

bool is_probable_prime(mpz_class m, int trials) {
    if (mpz_probab_prime_p(m.get_mpz_t(), trials) == 0)
        return false;
    return true;
}

mpz_class suyama(mpz_class p, curve_point &P) {
    gmp_randclass r(gmp_randinit_default);
    r.seed(time(NULL));
    mpz_class sigma = r.get_z_bits(64) + 5;
    mpz_class u, v, A;
    u = sigma*sigma - 5;
    v = 4*sigma;

    P.X = (u*u*u) % p;
    P.Z = (v*v*v) % p;

    A = ((v-u)*(v-u)*(v-u)*(3*u+v)/(4*u*u*u*v) - 2) % p;
    return A;
}

mpz_class ECMTrial(mpz_class N, long bound) {
    mpz_class A, g;
    mpz_t k, l, prime, boundGMP;

    mpz_inits(k, l, prime, boundGMP, NULL);

    curve_point P, Q;
    A = suyama(N, P);

    if (gcd(A*A - 4, N) != 1) {
        return gcd(A*A - 4, N);
    }

    Q.X = P.X;
    Q.Z = P.Z;

    int i = 0;

    while (i < 1229 && primes[i] < bound) {
        mpz_set_ui(k,log(bound) / log(primes[i]));
        mpz_set_ui(l, primes[i]);
        mpz_powm(l, l, k, N.get_mpz_t());
        Q = montgomeryLadder((mpz_class)l, Q, (A+2)/4, N);
        i++;
    }
    if (i == 1229) {
        mpz_set_ui(prime, primes[1228]);
        mpz_set_ui(boundGMP, bound);
        mpz_nextprime(prime, prime);
        while (mpz_cmp(prime, boundGMP) < 0 ) {
            mpz_set_ui(k,log(bound) / log(mpz_get_ui(prime)));
            mpz_powm(l, prime, k, N.get_mpz_t());
            Q = montgomeryLadder((mpz_class)l, Q, (A+2)/4, N);
            mpz_nextprime(prime, prime);
        }
    }

    mpz_clears(k, l, prime, boundGMP, NULL);

    g = gcd(Q.Z, N);
    if (g > 1 && g < N) {
        std::cout << "B : " << bound << std::endl;
        std::cout << "A : " << A << std::endl;
        std::cout << "P.X : " << P.X << std::endl;
        std::cout << "P.Z : " << P.Z << std::endl;
        return g;
    }

    return -1;
}

void factorization(mpz_class N, int NbEcmTrials, long bound) {

    std::vector<std::pair<int, int>> factors;

    if (trial_division(N, factors)) {
        for (auto factor : factors) {
            std::cout << factor.first << "^" << factor.second << std::endl;
            for (int i = 1; i <= factor.second; ++i)
                N /= factor.first;
        }
    }

    mpz_class factor;

    int i = 0;
    
    while (N > 1 && i < NbEcmTrials) {
        factor = ECMTrial(N, bound);
        if (factor != -1 && factor != N) {
            if (is_probable_prime(factor, 30))
                std::cout << factor << std::endl;
            else
                factorization(factor, NbEcmTrials, bound);
            N /= factor;
        }
        ++i;
        if (is_probable_prime(N, 30)) {
            break;
        }
    }
    std::cout << N << std::endl;
}

int main(int argc, char const *argv[])
{
    // eratostheneSieve(10000);

    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " Number_ECM_trials Bound Number_to_factor" << std::endl;
        return 1;
    }

    mpz_class N(argv[3]);
    std::cout << N << " = " << std::endl;
    factorization(N, atoi(argv[1]), atol(argv[2]));
    std::cout << std::endl;

    return 0;
}
