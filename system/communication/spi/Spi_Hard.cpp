/* 
* Spi_Hard.cpp
*
* Created: 25.04.2024 13:12:16
* Author: User
*/


#include "Spi_Hard.h"
#include "core/core.h"

#define		F_TRANSFER	45056
#define		MODE_TRANSFER	mode0;

#define		SPI_SCK_DDR			bit_is_byte(DDRB).bit1
#define		SPI_SCK				bit_is_byte(PORTB).bit1

#define		SPI_MOSI_DDR		bit_is_byte(DDRB).bit2
#define		SPI_MOSI			bit_is_byte(PORTB).bit2

#define		SPI_MISO_DDR		bit_is_byte(DDRB).bit3
#define		SPI_MISO			bit_is_byte(PINB).bit3

#define		SPI_SPIF			bit_is_byte(SPSR).bit7
#define		SPI_WCOL			bit_is_byte(SPSR).bit6
#define		SPI_2X				bit_is_byte(SPSR).bit0

#define		SPI_SPIE			bit_is_byte(SPCR).bit7
#define		SPI_SPE				bit_is_byte(SPCR).bit6
#define		SPI_DORD			bit_is_byte(SPCR).bit5
#define		SPI_MSTR			bit_is_byte(SPCR).bit4
#define		SPI_CPOL			bit_is_byte(SPCR).bit3
#define		SPI_CPHA			bit_is_byte(SPCR).bit2
#define		SPI_SPR1			bit_is_byte(SPCR).bit1
#define		SPI_SPR0			bit_is_byte(SPCR).bit0

#define		SPI_DIV_2			0b100
#define		SPI_DIV_4			0b000
#define		SPI_DIV_8			0b101
#define		SPI_DIV_16			0b001
#define		SPI_DIV_32			0b110
#define		SPI_DIV_64			0b010
#define		SPI_DIV_128			0b011

enum SPI_MODE
{
	mode0	= 0,
	mode1	= 1,
	mode2	= 2,
	mode3	= 3
};

inline void	SPI_SET_DIV(uint8_t div)
{
	SPI_2X		= bit_is_byte(div).bit2;
	SPI_SPR1	= bit_is_byte(div).bit1;
	SPI_SPR0	= bit_is_byte(div).bit0;
}

inline void	SPI_SET_MODE(SPI_MODE mode)
{
	SPI_CPOL	= bit_is_byte(mode).bit1;
	SPI_CPHA	= bit_is_byte(mode).bit0;
}

Spi_Hard	*Spi_Hard::obj	= 0;


Spi_Hard*	Spi_Hard::init()
{
	if (obj == 0)	obj = new Spi_Hard;
	//
	SPI_SPIE	= 0;			// запрет прерывания
	SPI_DORD	= 0;			// порядок передоваемых бит
	SPI_MSTR	= 1;			// ведущее устройство
	SPI_SET_MODE(mode0);		// режим 0
	// скорость обмена в коэффицент деления
	uint8_t div = 16;//(F_CPU / 1000) / F_TRANSFER;
	if (div >= 128)		SPI_SET_DIV(SPI_DIV_128);
	else
	if (div >= 64)		SPI_SET_DIV(SPI_DIV_64);
	else
	if (div >= 32)		SPI_SET_DIV(SPI_DIV_32);
	else
	if (div >= 16)		SPI_SET_DIV(SPI_DIV_16);
	else
	if (div >= 8)		SPI_SET_DIV(SPI_DIV_8);
	else
	if (div >= 4)		SPI_SET_DIV(SPI_DIV_4);
	else				SPI_SET_DIV(SPI_DIV_2);
	//
	SPI_SS			= 1;
	SPI_SCK			= 0;
	SPI_MOSI		= 0;
	SPI_MISO		= 0;
	//
	SPI_SS_DDR		= 1;
	SPI_SCK_DDR		= 1;
	SPI_MOSI_DDR	= 1;
	SPI_MISO_DDR	= 0;
	//
	SPI_SPE			= 1;
	return	obj;
}

// default constructor
Spi_Hard::Spi_Hard()
{
} //Spi_Hard

// default destructor
Spi_Hard::~Spi_Hard()
{
} //~Spi_Hard

uint8_t	Spi_Hard::getInByte()
{
	return	inByte;
}

uint8_t	Spi_Hard::transferByte(uint8_t byte)
{
	uint32_t	timeOut = 100000;
	SPDR	= byte;
	while(!SPI_SPIF)
	{
		if ((--timeOut) == 0)
		{
			statTransferByte = 1;
			return	0;
		}
	}
	inByte	= SPDR;
	statTransferByte	= 0;
	return	inByte;
}

