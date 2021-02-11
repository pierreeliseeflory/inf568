#include <iostream>
#include "montgomery.hpp"

curve_point xADD(curve_point P, curve_point Q, curve_point diff, mpz_class p) {
    mpz_class U, V;

    U = ((P.X - P.Z) * (Q.X + Q.Z)) % p;
    V = ((P.X + P.Z) * (Q.X - Q.Z)) % p;

    curve_point res;
    res.X = (diff.Z * (U + V) * (U + V)) % p;
    res.Z = (diff.X * (U - V) * (U - V)) % p;

    return res;
}

curve_point xDBL(curve_point P, mpz_class A24, mpz_class p) {
    mpz_class Q, R, S;
    
    Q = ((P.X + P.Z) * (P.X + P.Z)) % p;
    R = ((P.X - P.Z) * (P.X - P.Z)) % p;
    S = (Q - R) % p;

    curve_point res;
    res.X = (Q * R) % p;
    res.Z = (S * (R + S * A24)) % p;

    return res;
}

void swap(int b, montgomery_rung &state) {
    curve_point x0, x1;
    x0 = state.x0; 
    x1 = state.x1; 

    state.x0.X = (1-b) * x0.X + b * x1.X;
    state.x0.Z = (1-b) * x0.Z + b * x1.Z;
    state.x1.X = (1-b) * x1.X + b * x0.X;
    state.x1.Z = (1-b) * x1.Z + b * x0.Z;
}

mpz_class divide(mpz_class x, mpz_class z, mpz_class p) {
    mpz_class res, tmp = p - 2;
    mpz_powm(res.get_mpz_t(), z.get_mpz_t(), tmp.get_mpz_t(), p.get_mpz_t());
    return (x*res)%p;
}

curve_point montgomeryLadder (mpz_class m, curve_point P, mpz_class A24, mpz_class p) {
    montgomery_rung state;
    state.x0 = xDBL(P, A24, p);
    state.x1 = P;

    std::string decomposition = m.get_str(2);
    int beta = decomposition.size() - 1;


    for (int i = beta - 1; i >= 0; --i) {
        swap((decomposition[beta - i] - '0') ^ (decomposition[beta - (i+1)] - '0'), state);
        state.x1 = xADD(state.x0, state.x1, P, p);
        state.x0 = xDBL(state.x0, A24, p);
    }
    swap(decomposition[beta] - '0', state);

    return state.x0;
}