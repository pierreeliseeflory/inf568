# Poly1305

Pierre-Elisée Flory

## Building

`make` to compile poly1305-gen and poly1305-check.

## Files

### poly1305-core

Implements poly1305 according to the RFC. Uses the clamping function from D. Bernstein. Contains also the test vector presented in the RFC.

### poly1305-gen

Reads an hexadecimal key and the content of a file and creates an authenticator using poly1305-core.

### poly1305-check

Reads an hexadecimal key, the content of a file and an authenticator to check that the message is wellauthenticated.
