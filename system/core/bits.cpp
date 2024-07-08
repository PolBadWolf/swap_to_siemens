/*
 * bits.cpp
 *
 * Created: 04.04.2024 9:03:26
 *  Author: User
 */ 

#include <avr/io.h>


void	bit_set_byte(volatile	void	*byte, const unsigned char bit, const bool z) {
	if (z)	{
		(*(unsigned char *)byte) |= (1 << bit);
	}
	else {
		(*(unsigned char *)byte) &= ~(1 << bit);
	}
	
}

void	bit_xor_byte(volatile	unsigned char	*byte, const unsigned char bit) {
	(*byte) ^= (1 << bit);
}

uint8_t	odd_from_7bit(uint8_t dat)
{
	dat &= 0x7f;
	dat ^= dat >> 4;
	dat ^= dat >> 2;
	dat ^= dat >> 1;
	return dat & 0x01;
}

uint8_t odd_plus_7bit(uint8_t dat)
{
	dat &= 0x7f;
	uint8_t x = dat;
	x ^= x >> 4;
	x ^= x >> 2;
	x ^= x >> 1;
	return dat | ((x & 0x01) << 7);
}
