#include <iostream>
#include "montgomery.hpp"

/* 
Function Ladder
(x_0 , x_1 ) ← (x(0), x(P))
for i in (β − 1, . . . , 0) do
    if m_i = 0 then
        (x_0 , x_1 ) ← (xDBL(x_0 ), xADD(x_0 , x_1 , x(P))
    else
        (x_0 , x_1 ) ← (xADD(x_0 , x_1 , x(P)), xDBL(x_1 ))
return x_0


Function SWAP
    Input: b ∈ {0, 1} and (x_0 , x_1 )
    Output: (x_0 , x_1 ) if b = 0, (x_1 , x_0 ) if b = 1
    v ← b and (x_0 xor x_1 )
    return ((1 − b)x_0 + bx_1 , bx_0 + (1 − b)x_1 )


Function xADD
    Input: Xp Zp Xq Zq X_ Z_
    (X+ : Z+) = (z_ * [U + V]² : X_ * [U - V]²)
    where:
        U = (Xp - Zp)(Xq + Zq)
        V = (Xp + Zp)(Xq - Zq)

Function xDBL
    Input: Xp Zp
    (X2p : Z2p) = (Q * R : S (R + S * (A + 2)/4))
    where:
        Q = (Xp + Zp)²
        R = (Xp - Zp)²
        S = 4Xp * Zp = Q - R
*/

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

/*
Input: b ∈ {0, 1} and (x_0 , x_1 )
Output: (x_0 , x_1 ) if b = 0, (x_1 , x_0 ) if b = 1
    v ← b and (x_0 xor x_1 )
    return ((1 − b)x_0 + bx_1 , bx_0 + (1 − b)x_1 )
*/
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

mpz_class montgomeryLadder (mpz_class m, curve_point P, mpz_class A24, mpz_class p) {
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

    return divide(state.x0.X, state.x0.Z, p);
}

void testMontgomery() {
    mpz_class x,result;
    curve_point output, input, tmp;
    mpz_class p, A24;

    std::cout << "Test n°1" << std::endl;
    p = 101;
    A24 = 38;
    x = 2;
    input.X = x;
    input.Z = 1;
    result = montgomeryLadder(1, input, A24, p);
    std::cout << "[1]P = " << result << std::endl;
    result = montgomeryLadder(2, input, A24, p);
    std::cout << "[2]P = " << result << std::endl;
    output = xDBL(input, A24, p);
    output.X = divide(output.X, output.Z, p);
    output.Z = 1;
    std::cout << "[2]P = " << output.X << "  //xDBL"  << std::endl;
    output = xADD(input, output, input, p);
    std::cout << "[3]P = " << divide(output.X, output.Z, p) << "    //xADD" << std::endl;
    result = montgomeryLadder(3, input, A24, p);
    std::cout << "[3]P = " << result << std::endl;
    result = montgomeryLadder(77, input, A24, p);
    std::cout << "[77]P = " << result << std::endl;
    std::cout << "=================" << std::endl;


    std::cout << "Test n°2" << std::endl;
    p = 1009;
    A24 = 171;
    x = 7;
    result = montgomeryLadder(1, input, A24, p);
    std::cout << "[1]P = " << result << std::endl;
    result = montgomeryLadder(2, input, A24, p);
    std::cout << "[2]P = " << result << std::endl;
    output = xDBL(input, A24, p);
    output.X = divide(output.X, output.Z, p);
    output.Z = 1;
    std::cout << "[2]P = " << output.X << "  //xDBL"  << std::endl;
    output = xADD(input, output, input, p);
    std::cout << "[3]P = " << divide(output.X, output.Z, p) << "    //xADD" << std::endl;
    result = montgomeryLadder(3, input, A24, p);
    std::cout << "[3]P = " << result << std::endl;
    result = montgomeryLadder(5, input, A24, p);
    std::cout << "[5]P = " << result << std::endl;
    result = montgomeryLadder(34, input, A24, p);
    std::cout << "[34]P = " << result << std::endl;
    result = montgomeryLadder(104, input, A24, p);
    std::cout << "[104]P = " << result << std::endl;
    result = montgomeryLadder(947, input, A24, p);
    std::cout << "[947]P = " << result << std::endl;
    std::cout << "=================" << std::endl;

    std::cout << "Test n°3" << std::endl;
    p = (((mpz_class) 1) << 255) - 19;;
    A24 = 121666;
    x = 9;
    result = montgomeryLadder(2, input, A24, p);
    std::cout << "[2]P = " << result << std::endl;
    output = xDBL(input, A24, p);
    output.X = divide(output.X, output.Z, p);
    output.Z = 1;
    std::cout << "[2]P = " << output.X << "  //xDBL"  << std::endl;
    output = xADD(output, input, input, p);
    std::cout << "[2]P + P = " << divide(output.X, output.Z, p) << std::endl;
    result = montgomeryLadder(3, input, A24, p);
    std::cout << "[3]P = " << result << std::endl;
    result = montgomeryLadder(4, input, A24, p);
    std::cout << "[4]P = " << result << std::endl;
    result = montgomeryLadder(2, input, A24, p);
    output.X = result;
    output.Z = 1;
    result = montgomeryLadder(2, output, A24, p);
    std::cout << "[2][2]P = " << result << std::endl;
    result = montgomeryLadder(3, input, A24, p);
    output.X = result;
    output.Z = 1;
    tmp.X = montgomeryLadder(2, input, A24, p);
    tmp.Z = 1;
    output = xADD(output, input, tmp, p);
    std::cout << "[3]P + P = " << divide(output.X, output.Z, p) << std::endl;
    result = montgomeryLadder(5, input, A24, p);
    std::cout << "[5]P = " << result << std::endl;
    result = montgomeryLadder(7, input, A24, p);
    std::cout << "[7]P = " << result << std::endl;
    std::cout << "=================" << std::endl;
}