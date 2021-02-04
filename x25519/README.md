# X25519

I used the GMP C++ interface (in particular mpz_class that helps in implementing quickly). I had to use the mpz_t projection to compute integer exponentiation in the divide function.

I let the function that compute all the tests from *test/test_vectors_montgomery* in *montgomery.cpp*. The tests of the RFC have been used to debug the code and to validate the implementation.

## Code

### Montgomery ladder

The elliptic curve : $E: BY^2Z = X(X^2 + AXZ + Z^2)$ sur $GF(p)$

* p = 2^255 - 19
* A = 486662
* B = 1

#### ladder(m,(X,Z))$

##### Input

* $m$: an integer (scalar multiple)
* $(X,Z)$ : integers modulo $p$

##### Output

* $(X_m,Z_m)$ : integers modulo $p$ such that $(X_m:*:Z_m) = [m](X:*:Z)$ on the curve

##### Montgomery

The xDBL function use the formula from *Montgomery curves and their arithmetic* and not the one proposed by Berstein.

I implemented the swap function to avoid conditionnal branching on the value of m. It's usage in the Montgomery Ladder comes from the pseudo-code presented in the paper *Montgomery curves and their arithmetic*.

### X25519 curve

The scalar clamping function has been implemented according to Bernstein's code.

The coordinate decoding function has been taken from the RFC.

The I/O are handled by outputHexa and inputHexa.

## X25519 key exchange

hex-encoded string of 32 bytes

### 1 arg

DH key generation -> m

### 2 args

DH key exchange -> m, u(preprocessing) optional else 9

## Build

```bash
make
```

## Use

```bash
./x25519 secret_key [public_key]
```
