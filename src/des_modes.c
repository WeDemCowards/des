#include <endian.h>
#include <stdio.h>
#include <stdint.h>

#include "des.h"

int des_ecb_enc(FILE *in_fd, FILE *out_fd, uint64_t ks[16]) {
	uint64_t message;
	uint64_t ciphertext;
	size_t bytes_read;

	// Encrypt and write WHOLE BLOCKS
	for ( message = 0; (bytes_read = fread(&message, 1, sizeof(message), in_fd)) == sizeof(message); message = 0) {
		message = htole64(message);
		ciphertext = des(message, ks);
		ciphertext = le64toh(ciphertext);
		if (fwrite(&ciphertext, 1, sizeof(ciphertext), out_fd) != sizeof(ciphertext)) {
			fprintf(stderr, "error: couldn't write the entire ciphertext block\n");
		}
	}
	// Arrange padded message
	uint8_t pad_len = sizeof(message) - bytes_read;
	for (size_t i=sizeof(message); i>bytes_read; i--) {
		message |= (uint64_t)pad_len << (8 * (i-1));
	}
	// Encrypt and write the PADDED BLOCK
	ciphertext = des(message, ks);
	ciphertext = le64toh(ciphertext);
	if (fwrite(&ciphertext, 1, sizeof(ciphertext), out_fd) != sizeof(ciphertext)) {
		fprintf(stderr, "error: couldn't write the entire ciphertext block\n");
	}
	return 0;
}

int des_ecb_dec(FILE *in_fd, FILE *out_fd, uint64_t ks[16]) {
	uint64_t ciphertext;
	uint64_t plaintext;
	size_t bytes_read;

	bytes_read = fread(&ciphertext, 1, sizeof(ciphertext), in_fd);
	ciphertext = htole64(ciphertext);
	plaintext = des(ciphertext, ks);

	// Write each decrypted block to the output, except for the last one
	for (   ciphertext = 0;
			(bytes_read = fread(&ciphertext, 1, sizeof(ciphertext), in_fd)) == sizeof(ciphertext);
			ciphertext = 0 )
	{
		ciphertext = htole64(ciphertext);
		plaintext = le64toh(plaintext);
		if (fwrite(&plaintext, 1, sizeof(plaintext), out_fd) != sizeof(plaintext))
			fprintf(stderr, "des_ecb_dec(): failed to write the entire plaintext block. Decryption may be corrupted.\n");

		plaintext = des(ciphertext, ks);
	}

	// We can now expect plaintext to hold the padding block.
	size_t pad_len = plaintext >> (8 * 7);
	for (size_t i=7; i>8-pad_len; i--) {
		uint8_t pad_byte = plaintext >> (8 * (i-1));
		if (pad_byte != pad_len) {
			fprintf(stderr, "des_ecb_dec(): The padding bytes are not consistent. %lu != %u. Decryption may be corrupted.\n", pad_len, pad_byte);
		}
	}
	// Write final block to output
	plaintext = le64toh(plaintext);
	if (fwrite(&plaintext, 1, (sizeof(plaintext) - pad_len), out_fd) != (sizeof(plaintext) - pad_len))
			fprintf(stderr, "des_ecb_dec(): failed to write the entire plaintext block. Decryption may be corrupted.\n");
	
	// Return error if message length not a multiple of 8 bytes
	if (bytes_read != 0) {
		fprintf(stderr, "des_ecb_dec(): ciphertext does not fit expected alignment (64 bits)\n");
		fprintf(stderr, "               Expect the decryption to be corrupted.\n");
		return 1;
	}
	return 0;
}
