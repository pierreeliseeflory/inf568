# INF568 - Advanced Cryptology

1. [Hash functions - Keccak](#keccak-implementation-shake128)

## Keccak implementation SHAKE128

### Usage

1. Move inside the [shake128](shake128) directory.
2. Run the default Makefile. 
3. Run the binary:

> bin/shake128 output_size_in_bytes < file_to_be_hashed

#### Example

```bash
$ cd shake128/
$ make
$ bin/shake128 32 < test/test.bin
407B0E00C6CE9548E1ECC8482E67E38F78EBA7E5702C92F74EDDE8DEE3B9BCE4
$ bin/shake128 200
Test string from stdin. # ctrl-D to end input (twice if not at the beginning of a line)
2A02665669DB36362344B0F39FF683A6ED28A845FE87[...]CAF4DCFECA5F249615
```