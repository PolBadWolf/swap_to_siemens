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
