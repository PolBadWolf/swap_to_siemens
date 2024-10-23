/*
 * bits.h
 *
 * Created: 03.04.2024 14:07:08
 *  Author: User
 */ 


#ifndef BITS_H_
#define BITS_H_

enum EISR
{
	LowLevel	= 0
	,TwoLevel	= 1
	,DelLevel	= 2
	,HighLevel	= 3
};


#define bit_is_byte(x)		(*((volatile BYTE_BITFIELD*)(&x)))
#define bit_is_word(x)		(*((volatile WORD_BITFIELD*)(&x)))
#define word_to_byte(x)		(*((volatile DOUBLE_BYTE*)(&x)))
#define dWord_to_byte(x)	(*((volatile TETRA_BYTE*)(&x)))
#define set_EISR_A			(*((volatile EISR_A_BITFIELD*)(&EICRA)))
#define set_EISR_B			(*((volatile EISR_B_BITFIELD*)(&EICRB)))

typedef struct {
	unsigned	EISC4:2;
	unsigned	EISC5:2;
	unsigned	EISC6:2;
	unsigned	EISC7:2;
} EISR_B_BITFIELD;

typedef struct {
	unsigned	EISC0:2;
	unsigned	EISC1:2;
	unsigned	EISC2:2;
	unsigned	EISC3:2;
} EISR_A_BITFIELD;

typedef struct {
	unsigned bit0:1;
	unsigned bit1:1;
	unsigned bit2:1;
	unsigned bit3:1;
	unsigned bit4:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
} BYTE_BITFIELD;

typedef struct {
	unsigned bit0:1;
	unsigned bit1:1;
	unsigned bit2:1;
	unsigned bit3:1;
	unsigned bit4:1;
	unsigned bit5:1;
	unsigned bit6:1;
	unsigned bit7:1;
	unsigned bit8:1;
	unsigned bit9:1;
	unsigned bit10:1;
	unsigned bit11:1;
	unsigned bit12:1;
	unsigned bit13:1;
	unsigned bit14:1;
	unsigned bit15:1;
} WORD_BITFIELD;

typedef struct
{
	unsigned Low:4;
	unsigned High:4;
} BYTE_TETRA;

typedef struct
{
	unsigned Low:8;
	unsigned High:8;
} DOUBLE_BYTE;

typedef struct
{
	unsigned Byte1:8;
	unsigned Byte2:8;
	unsigned Byte3:8;
	unsigned Byte4:8;
} TETRA_BYTE;

// inline	
void	bit_set_byte(volatile	void	*byte, const unsigned char bit, const bool z);
void	bit_xor_byte(volatile	unsigned char	*byte, const unsigned char bit);

uint8_t	odd_from_7bit(uint8_t dat);
uint8_t odd_plus_7bit(uint8_t dat);

#endif /* BITS_H_ */