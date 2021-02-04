# Poly1305

1. poly1305-gen
2. poly1305-check

## Files

### poly1305-gen

#### Arguments

1. A 32-byte key, given as a 64-character hexadecimal string (which will be split into r and s)
2. The name of the file to authenticate.

#### Output

It should write a 16-byte authenticator to standard output, as a 32-character hexadecimal string.

### poly1305-check

#### Arguments

1. A 32-byte key, given as a 64-character hexadecimal string (which will be split into r and s)
2. The name of the file to authenticate
3. A 16-byte authenticator tag, given as a 32-character hexadecimal string.

#### Output

It should write out either ACCEPT or REJECT, according to whether the authenticator is valid or not.

### Technical notes

- The nonce (or message number) is not included as an argument to either program: it is implicit in the s-value.
- Byte ordering, endianness, etc., should be as in the RFC.
- The RFC includes a test - which you should definitely pass!
- You do not need to use AES or ChaCha20 to generate the secret values: the user is allowed to generate them any way that they like.  (It is therefore essential to follow the clamping procedure mentioned in the RFC, because there is no assumption that the r-value derived from the argument will respect the special restrictions.)

RFC section 2.5

GMP (or NTL) -> Finite field arithmetics

Bernstein Poly1305 high speed methods