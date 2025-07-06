#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "des.h"
#include "keygen.h"

#include "utils.h"

enum Function {
	UNSPECIFIED,
	ENCRYPT,
	DECRYPT,
	KEYGEN,
};
enum Mode {
	ECB,
	CBC,
};

int init_des(enum Function function, enum Mode mode, FILE *in_fd, FILE *out_fd, FILE *key_fd);
int des_ecb_enc(FILE *in_fd, FILE *out_fd, uint64_t ks[16]);
int des_ecb_dec(FILE *in_fd, FILE *out_fd, uint64_t ks[16]);
void print_usage();



int main(int argc, char **argv) {
	enum Function function = UNSPECIFIED;
	enum Mode mode = ECB;
	
	char *in_fp  = NULL;
	char *out_fp = NULL;
	char *key_fp = NULL;
	
	// Print help if no args are passed
	if (argc < 2) {
		print_usage();
		return 1;
	}
	
	// Check argv[1] for FUNCTIONS
	if (0) ;
	else if ((strcmp(argv[1], "-e") == 0) || (strcmp(argv[1], "--encrypt") == 0)) {
		function = ENCRYPT;
	}
	else if ((strcmp(argv[1], "-d") == 0) || (strcmp(argv[1], "--decrypt") == 0)) {
		function = DECRYPT;
	}
	else if ((strcmp(argv[1], "-g") == 0) || (strcmp(argv[1], "--keygen") == 0)) {
		function = KEYGEN;
	}
	else if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0)) {
		print_usage();
		return 0;
	}
	else {
		fprintf(stderr, "error: invalid FUNCTION \"%s\"\n", argv[1]);
		return 1;
	}
	
	// Check the rest of the OPTIONS
	char next_arg = 0; // non-zero indicates we are reading an option's value next.
	for (int i=2; i<argc; i++) {
		if (next_arg == 0) {
			if (0) ;
			else if ((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--input") == 0) || (strcmp(argv[i], "-in") == 0)) {
				next_arg = 'i';
			}
			else if ((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "--output") == 0) || (strcmp(argv[i], "-out") == 0)) {
				next_arg = 'o';
			}
			else if ((strcmp(argv[i], "-k") == 0) || (strcmp(argv[i], "--key") == 0)) {
				next_arg = 'k';
			}
			else if ((strcmp(argv[i], "-m") == 0) || (strcmp(argv[i], "--mode") == 0)) {
				next_arg = 'm';
			}
			else {
				fprintf(stderr, "error: invalid option \"%s\"\n", argv[i]);
				return 1;
			}
		}
		else if (next_arg == 'i') {
			in_fp = argv[i];
			next_arg = 0;
		}
		else if (next_arg == 'o') {
			out_fp = argv[i];
			next_arg = 0;
		}
		else if (next_arg == 'k') {
			key_fp = argv[i];
			next_arg = 0;
		}
		else if (next_arg == 'm') {
			if (0) ;
			else if (strcmp(argv[i], "ecb") == 0) {
				mode = ECB;
				next_arg = 0;
			}
			else if (strcmp(argv[i], "cbc") == 0) {
				mode = CBC;
				next_arg = 0;
			}
			else {
				fprintf(stderr, "error: invalid option for mode of operation: \"%s\"", argv[i]);
				return 1;
			}
		}
		else {
			fprintf(stderr, "critical error: invalid next_char %c\n", next_arg);
			return 1;
		}
	}
	
	// At this point, we have received our arguments. Now we open our files up.
	FILE *in_fd  = NULL;
	FILE *out_fd = NULL;
	FILE *key_fd = NULL;
	
	// Open in_fd and key_fd if the function is encryption/decryption
	if (function == ENCRYPT || function == DECRYPT) {
		// Open in_fd
		if (in_fp == NULL) {
			in_fd = stdin;
		}
		else {
			if ((in_fd = fopen(in_fp, "rb")) == NULL) {
				fprintf(stderr, "error: could not open file \"%s\" for reading\n", in_fp);
				return 1;
			}
		}
		// Open key_fd
		if (key_fp == NULL) {
			fprintf(stderr, "error: a keyfile (-k) must be provided for encryption/decryption\n");
			return 1;
		}
		if ((key_fd = fopen(key_fp, "rb")) == NULL) {
			fprintf(stderr, "error: could not open file \"%s\" for reading\n", key_fp);
			return 1;
		}
	}
	// Open out_fd
	if (out_fp == NULL) {
		out_fd = stdout;
	}
	else {
		if ((out_fd = fopen(out_fp, "wb")) == NULL) {
			fprintf(stderr, "error: could not open file \"%s\" for writing\n", out_fp);
			return 1;
		}
	}
	
	// Now that we have our files open, we can start the program in the chosen function.
	if (0) ;
	// KEY GENERATION
	else if (function == KEYGEN) {
		uint64_t key;
		if (generate_des_key(&key)) {
			return 1;
		}
		if ((fwrite(&key, 1, sizeof(key), out_fd)) != sizeof(key)) {
			fprintf(stderr, "error: failed to write the correct data to the key. Delete your key and try again.\n");
			return 1;
		}
	}
	// ENCRYPTION & DECRYPTION
	else if (function == ENCRYPT || function == DECRYPT) {
		init_des(function, mode, in_fd, out_fd, key_fd);
	}
	
	// Close our files
	if (in_fd && in_fd != stdin) fclose(in_fd);
	if (out_fd && out_fd != stdout) fclose(out_fd);
	if (key_fd) fclose(key_fd);
	
	return 0;
}

// Drives the des encryption process
int init_des(enum Function function, enum Mode mode, FILE *in_fd, FILE *out_fd, FILE *key_fd) {
	uint64_t key = 0;
	uint64_t ks[16] = {0};
	// 1. Read key from file
	if ((fread(&key, 1, sizeof(key), key_fd)) != sizeof(key)) {
		fprintf(stderr, "error: failed to read the correct amount of data from key file.\n");
		return 1;
	}
	// 2. Validate key (can disable by flipping conditional below, if you wish to use unvalidated keys)
	if (1) {
		if (count_even_bytes(key)) {
			fprintf(stderr, "error: provided key does not have odd byte parity.\n");
			return 1;
		}
	}
	// 3. Key Scheduling
	schedule_keys(key, ks);
	if (function == DECRYPT) {
		for (int i=0; i<8; i++) {
			uint64_t tmp = ks[i];
			ks[i] = ks[15-i];
			ks[15-i] = tmp;
		}
	}
	// 4. Modes
	if (0) ;
	else if (mode == ECB) {
		if (function == ENCRYPT) des_ecb_enc(in_fd, out_fd, ks);
		if (function == DECRYPT) des_ecb_dec(in_fd, out_fd, ks);
	}
	else if (mode == CBC) {
		fprintf(stderr, "CBC not implemented. Sorry!\n");
	}
	return 0;
}

int des_ecb_enc(FILE *in_fd, FILE *out_fd, uint64_t ks[16]) {
	uint64_t message;
	uint64_t ciphertext;
	size_t bytes_read;

	// Encrypt and write WHOLE BLOCKS
	for ( message = 0; (bytes_read = fread(&message, 1, sizeof(message), in_fd)) == sizeof(message); message = 0) {
		ciphertext = des(message, ks);
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
	plaintext = des(ciphertext, ks);

	// Write each decrypted block to the output, except for the last one
	for (   ciphertext = 0;
			(bytes_read = fread(&ciphertext, 1, sizeof(ciphertext), in_fd)) == sizeof(ciphertext);
			ciphertext = 0 )
	{
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

void print_usage() {
	printf("Usage: des FUNCTION [OPTIONS]\n");
	printf("\n");
	printf("FUNCTION\n");
	printf("    -e, --encrypt                       Encrypt the input data (Requires a keyfile)\n");
	printf("    -d, --decrypt                       Decrypt the input data (Requires a keyfile)\n");
	printf("    -g, --keygen                        Generate a des key\n");
	printf("    -h, --help                          Print this help page\n");
	printf("\n");
	printf("OPTIONS\n");
	printf("    -i <filepath>, --input <filepath>   Specify the input for the cipher\n");
	printf("    -o <filepath>, --output <filepath>  Specify the output for the cipher or keygen\n");
	printf("    -k <filepath>, --key <filepath>     Specify the key for the cipher\n");
	printf("    -m <mode>,     --mode <mode>        Specify blockcipher mode of operation.\n");
	printf("\n");
	printf("MODES\n");
	printf("    ecb     Electronic Codebook\n");
	printf("    cbc     Cipher Block Chaining\n");
	printf("\n");
	printf("DEFAULT BEHAVIOUR\n");
	printf("    Specifying a FUNCTION is necessary. If no function is specified, then this\n");
	printf("    help page will be printed.\n");
	printf("\n");
	printf("    If keygen is selected, the only relevant option is -o --output. If this is\n");
	printf("    not specified, the generated key will be written to stdout.\n");
	printf("\n");
	printf("    If encryption or decryption are selected, a filepath for the key must be\n");
	printf("    provided.\n");
	printf("\n");
	printf("    If the following options are not provided, their default behaviour is as follows: \n");
	printf("        - mode:   ecb\n");
	printf("        - input:  stdin\n");
	printf("        - output: stdout\n");
	printf("\n");
	printf("AUTHOR\n");
	printf("    Aaron McCurdy\n");
	printf("    mccurdya@proton.me\n");
	printf("    https://github.com/WeDemCowards\n");
}
