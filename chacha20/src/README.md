# AEAD Chacha20 / Poly1305

Pierre-Elisée Flory

## Building

`make` to compile aead_chacha20_poly1305.

## Files

### poly1305-core

Implements poly1305 according to the RFC. Uses the clamping function from D. Bernstein. Contains also the test vector presented in the RFC.

### chacha20-core

Implements chacha20 encryption. Tests were performed in a different file.

OpenMP has been used to run in parallel a part of CHACHA20 Quarterround. Since no speedup was measured the final version doesn't use it.

### aead_chacha20-core_poly1305

Usage:

```shell
$ ./aead_chacha20_poly1305 key nonce
> [plaintext]
```

The program outputs the value in hexadecimal of the ciphertext, the AEAD construction and the tag.
