#ifndef KEYGEN_H
#define KEYGEN_H

#include <stdint.h>

/* count_bits: returns the hamming weight of a byte */
int count_bits(uint8_t byte);

/* count_even_bytes: returns the number of bytes with even parity in a 64 bit 
   integer */
int count_even_bytes(uint64_t n);

/* generate_des_key: fills int pointed to by key with random data, with parity 
   bits set. */
int generate_des_key(uint64_t *key);

#endif