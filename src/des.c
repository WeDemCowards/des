#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "des.h"
#include "tables.h"


//// Feistel Sub-functions

uint64_t expand(uint32_t input) {
	uint64_t result = 0;
	for (int i=0; i<48; i++) {
		result |= (uint64_t)((input >> (32 - E[i])) & 1) << (47 - i);
	}
	return result;
}

uint32_t substitute(uint64_t input) {
	uint32_t result = 0;
	for (int i=0; i<8; i++) {
		uint8_t word = (input >> (6 * (7 - i))) & ((1 << 6) - 1);
		uint8_t row = (((word >> 5) & 1) << 1) | (word & 1);
		uint8_t column = (word >> 1) & ((1 << 4) - 1);
		uint32_t substitution = S_BOXES[i][row][column];
		result |= substitution << (4 * (7 - i));
	}
	return result;
}

uint32_t permute(uint32_t input) {
	uint32_t result = 0;
	for (int i=0; i<32; i++) {
		result |= ((input >> (32 - P[i])) & 1) << (31 - i);
	}
	return result;
}

//// Feistel

uint32_t feistel(uint32_t r, uint64_t k) {
	uint64_t expansion = expand(r);
	expansion ^= k;
	r = substitute(expansion);
	r = permute(r);
	return r;
}

//// DES Round

void des_round(uint32_t *l, uint32_t *r, uint64_t key) {
	uint32_t next_l = *r;
	*r = feistel(*r, key) ^ *l;
	*l = next_l;
}

//// IP & INVERSE IP

uint64_t initial_permutation(uint64_t message){
	uint64_t result = 0;
	for (int i=0; i<64; i++) {
		result |= ((message >> (64 - IP[i])) & 1ULL) << (63 - i);
	}
	return result;
}

uint64_t inverse_initial_permutation(uint64_t message) {
	uint64_t result = 0;
	for (int i=0; i<64; i++) {
		result |= ((message >> (64 - INVERSE_IP[i])) & 1ULL) << (63 - i);
	}
	return result;
}

//// DES

uint64_t des(uint64_t message, uint64_t ks[16]) {
	message = initial_permutation(message);
	uint32_t l = (uint32_t)(message >> 32);
	uint32_t r = (uint32_t)(message & 0xFFFFFFFF);
	for (int i=0; i<16; i++) {
		des_round(&l, &r, ks[i]);
	}
	uint32_t tmp = l;
	l = r;
	r = tmp;
	message = ((uint64_t)l << 32) | (uint64_t)r;
	return inverse_initial_permutation(message);
}

//// Key Scheduling

uint64_t pc1(uint64_t key) {
	uint64_t result = 0;
	for (int i=0; i<56; i++) {
		result |= ((key >> (64 - PC1[i])) & 1) << (55 - i);
	}
	return result;
}

uint64_t pc2(uint64_t input) {
	uint64_t result = 0;
	for (int i=0; i<48; i++){
		result |= ((input >> (56 - PC2[i])) & 1) << (47 - i);
	}
	return result;	
}

uint32_t left_rotate_28b(uint32_t b, int n) {
	const uint32_t mask = 0x0FFFFFFF;
	n = n % 28;
	b &= mask;
	return ((b << n) | (b >> (28 - n))) & mask;
}

void schedule_keys(uint64_t key, uint64_t ks[16]) {
	// Zero out key schedule (just in case)
	for (int i=0; i<16; i++) {
		ks[i] = 0;
	}
	// Permuted Choice 1
	key = pc1(key);
	
	// Split PC1 into C and D
	uint32_t c = (uint32_t)(key >> 28);
	uint32_t d = (uint32_t)(key & 0x000000000FFFFFFF);
	
	// Main Loop:
	for (int i=0; i<16; i++) {
		// Rotate and reunite C and D
		c = left_rotate_28b(c, SHIFTS[i]);
		d = left_rotate_28b(d, SHIFTS[i]);
		key = ((uint64_t)c << 28) | (uint64_t)d;
		
		// Permuted Choice 2 -> key schedule
		ks[i] = pc2(key);
	}
}


//// SINGLE TEST VECTOR
/*
int main(void) {
	uint64_t input = 0x0000000000000000;
	uint64_t key   = 0x10316E028C8F3B4A;
	uint64_t ks[16] = {0};
	
	schedule_keys(key, ks);
	uint64_t cipher = des(input, ks);

	
	printf("input: %" PRIx64 "\n", input);
	printf("key: %" PRIx64 "\n", key);
	printf("cipher: %" PRIx64 "\n", cipher);
	

}
*/

/*	input: 	0x0000000000000000
	key: 	0x10316e028c8f3b4a
	
		L        R
	00000000 47092B5B
	47092B5B 53F372AF
	53F372AF 9F1D158B
	9F1D158B 8109CBEE
	8109CBEE 60448698
	60448698 29EBB1A4
	29EBB1A4 620CC3A3
	620CC3A3 DEEB3D8A
	DEEB3D8A A1A0354D
	A1A0354D 9F0303DC
	9F0303DC FD898EE8
	FD898EE8 2D1AE1DD
	2D1AE1DD CBC829FA
	CBC829FA B367DEC9
	B367DEC9 3F6C3EFD
	3F6C3EFD 5A1E5228
	
	ciphertext: 82dcbafbdeab6602
	
	Source:
	VALIDATING THE CORRECTNESS OF HARDWARE IMPLEMENTATIONS OF THE NBS DATA ENCRYPTION STANDARD
	Written by Jason Gait
*/
