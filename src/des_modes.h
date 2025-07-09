#ifndef DES_MODES_H
#define DES_MODES_H

#include <stdio.h>
#include <stdint.h>

int des_ecb_enc(FILE *in_fd, FILE *out_fd, uint64_t ks[16]);
int des_ecb_dec(FILE *in_fd, FILE *out_fd, uint64_t ks[16]);
int des_cbc_enc(FILE *in_fd, FILE *out_fd, uint64_t ks[16]);
int des_cbc_dec(FILE *in_fd, FILE *out_fd, uint64_t ks[16]);

#endif
