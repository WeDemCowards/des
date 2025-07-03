#ifndef DES_H
#define DES_H

//// Feistel sub-functions

/* expand: Takes 32 bit input, expands to 48 bits. */
uint64_t expand(uint32_t input);

/* substitute: takes a 48 bit input, substitutes down to 32 bits */
uint32_t substitute(uint64_t input);

/* permute: takes a 32 bit input, returns a 32 bit permutation */
uint32_t permute(uint32_t input);

//// Feistel

/* feistel: takes 32 bit message r and 48 bit key, returns 32 bit cipher*/
uint32_t feistel(uint32_t r, uint64_t k);

//// DES Round
/* des_round: takes 32 bit l and r along with a 48 bit key. 
              updates l and r to l_n+1 and r_n+1 */
void des_round(uint32_t *l, uint32_t *r, uint64_t key);


//// IP and INVERSE IP

/* inital_permutation: takes a 64 bit message, returns a 64 bit permutation */
uint64_t inital_permutation(uint64_t m);

/* inverse_initial_permutation: takes a 64 bit message, returns a 64 bit permutation*/
uint64_t inverse_initial_permutation(uint64_t m);

//// DES

/* des: takes a 64 bit message and key schedule, returns a 64 bit ciphertext */
uint64_t des(uint64_t message, uint64_t ks[16]);

//// Key Scheduling

/* pc1: takes a 64 bit key, returns a 56 bit permutation*/
uint64_t pc1(uint64_t key);

/* pc2: takes a 56 bit input, returns a 48 bit permutation */
uint64_t pc2(uint64_t cd);

/* left_rotate_28b: rotates the 28 bit number b by n bits. masks out the four unused bits. */
uint32_t left_rotate_28b(uint32_t b, int n);

/* schedule_keys: takes 64 bit key (discards parity bits), produces key schedule ks. */
void schedule_keys(uint64_t key, uint64_t ks[16]);

#endif