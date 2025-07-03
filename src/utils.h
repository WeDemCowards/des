#include <stdio.h>
#include <stdint.h>

/////////////////////////////////////////////////////////////////////////
//	Functions added to this file should not be critical to the program //
//	They should just be used for debugging purposes.                   //
/////////////////////////////////////////////////////////////////////////

// Generic binary printer
void print_binary_uint(uint64_t value, int bits) {
    for (int i = bits - 1; i >= 0; i--) {
        putchar((value >> i) & 1 ? '1' : '0');
		if ((i) % 8 == 0)
			putchar(' ');
    }
    putchar('\n');
}

// Specific functions for each stdint type
void print_binary_int8(int8_t val)    { print_binary_uint((uint8_t)val, 8); }
void print_binary_uint8(uint8_t val)  { print_binary_uint(val, 8); }

void print_binary_int16(int16_t val)   { print_binary_uint((uint16_t)val, 16); }
void print_binary_uint16(uint16_t val) { print_binary_uint(val, 16); }

void print_binary_int32(int32_t val)   { print_binary_uint((uint32_t)val, 32); }
void print_binary_uint32(uint32_t val) { print_binary_uint(val, 32); }

void print_binary_int64(int64_t val)   { print_binary_uint((uint64_t)val, 64); }
void print_binary_uint64(uint64_t val) { print_binary_uint(val, 64); }
