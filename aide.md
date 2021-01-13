# SHAKE-128

SHAKE128 -> 5*5*64 -> b = 1600

b (1600 bits) and r (24 rounds) fixed

Keccak-f

Be careful of endianness

capacity c = security

Keccak[c] -> Keccak[512] -> security of 256 bits

KECCAK-p[1600, 24] = KECCAK-f[1600]

## Permutation

### theta

### rho

### pi

### chi

### iota

Parameterized by the round index

## Padding (pad10*1)

message padded with 4 1s + d the size of output wanted then padding

## Sponge
