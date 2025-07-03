#include <sys/random.h>
#include <stdio.h>
#include <stdint.h>

#include "keygen.h"

int count_bits(uint8_t byte) {
	int count = 0;
	while (byte > 0) {
		count += byte & 1;
		byte >>= 1;
	}
	return count;
}

int count_even_bytes(uint64_t n) {
	int count = 0;
	for (int i=0; i<8; i++) {
		uint8_t byte = (n >> (i * 8)) & 0x00000000000000FF;
		count += (count_bits(byte) % 2 == 0);
	}
	return count;
}

int generate_des_key(uint64_t *key) {
	// Fill key with random data
	if (getrandom(key, sizeof(*key), 0) != sizeof(*key)) {
		fprintf(stderr, "generate_des_key(): could not generate enough random data..\n");
		return 1;
	}
	// Adjust parity bits
	for (int i=0; i<8; i++) {
		uint8_t byte = (*key >> (i * 8)) & 0x00000000000000FF;
		if (count_bits(byte) % 2 == 0) {
			*key ^= (1ULL << (i * 8));
		}
	}
	// Confirm Parity
	if (count_even_bytes(*key)) {
		fprintf(stderr, "generate_des_key(): failed to create odd-parity for key\n");
		return 1;
	}
	return 0;
}
