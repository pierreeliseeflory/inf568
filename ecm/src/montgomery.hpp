#ifndef __MONTGOMERY_H__
#define __MONTGOMERY_H__

#include <gmpxx.h>

struct curve_point
{
    mpz_class X;
    mpz_class Z;
} typedef curve_point;

struct montgomery_rung
{
    curve_point x0;
    curve_point x1;
} typedef montgomery_rung;

// @Bernstein
void clamping(unsigned char);

curve_point xADD(curve_point, curve_point, curve_point, mpz_class);

curve_point xDBL(curve_point, mpz_class, mpz_class);

void swap(int, montgomery_rung &);

curve_point montgomeryLadder (mpz_class, curve_point, mpz_class, mpz_class);

mpz_class divide(mpz_class, mpz_class, mpz_class);

void testMontgomery();

#endif // __MONTGOMERY_H__