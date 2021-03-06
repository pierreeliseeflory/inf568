# Lenstra's ECM factorisation method

the field prime -> the number N to be factored
A -> varies in each ECM trial

* gcd
* trial_division(N) -> find any prime factors of N < 10000 -> sieve of Eratosthene
* is-probable_prime(m, ntrials) m passes ntrials of Miller-rabin
* factorisation(N)
  * calls trial_division
  * calls multiple time ECMTrial(N, A, bound)
* ECMTrial(N, A, bound)
  * generate X, Z modulo N such as X,Z projective point of the curve with A over Z/NZ
  * compute $(X_m,Z_m) = [m](X;*;Z)$ where m is the product of all prime powers less than bound
  * return $gcd(Z_m, N)$

<!--
factorization(N), which prints out as many prime factors of  N (with their exponents) as possible.  factorization will call trial_division first, to find any easy (tiny) factors, before making a number of calls to ECMTrial.


The main function that you need to code yourself is ECMTrial(N, A, bound), which should

Generate integers X and Z modulo N such that (X:*:Z) is a projective point on an elliptic curve BY^2Z = X(X^2 + AXZ + Z^2) over Z/NZ, using the given A (the y-coordinate * is irrelevant, as is the curve parameter B);

Compute (X_m,Z_m) such that (X_m:*:Z_m) = [m](X:*:Z) where m is the product of all prime powers less than bound;

Return gcd(Z_m, N).


B smooth vs B powersmooth

factor digits
40
45
50
55
60
65
suggested B 1
3 · 10 6
11 · 10 6
43 · 10 6
110 · 10 6
260 · 10 6
850 · 10 6
expected curves
2440
4590
7771
17899
43670
69351

 -->
