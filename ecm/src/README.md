# ECM Factorization

Pierre-Elisée Flory

## Content

Lenstra's ECM factorization method using the peviously implemented Montgomery Ladder.

Only the first stage of the algorithm as been implemented.

I used the proposed list of primes (< 10 000).

I didn't pay attention to the format required for factors and only saved the parameter A of the curve that found the factor. At the last minute I restarted some computations for the small numbers to try to use the updated version that outputs A, P.X, P.Y and the bound used. So in the end most factors found only have the curve used added.

## Usage

```shell
make
./ecm  Number_ECM_trials Bound Number_to_factor
```
