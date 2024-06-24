/* 
* Spi_Hard.h
*
* Created: 25.04.2024 13:12:16
* Author: User
*/


#ifndef __SPI_HARD_H__
#define __SPI_HARD_H__

#include "core/core.h"

#ifndef		SPI_SS_DDR
	#define		SPI_SS_DDR			bit_is_byte(DDRB).bit0
#endif

#ifndef		SPI_SS
	#define		SPI_SS				bit_is_byte(PORTB).bit0
#endif



class Spi_Hard
{
//variables
public:
	uint8_t	statTransferByte;
protected:
private:
	static	Spi_Hard	*obj;
	uint8_t	inByte;
//functions
public:
	static	Spi_Hard*	init();
	uint8_t	getInByte();
	uint8_t	transferByte(uint8_t byte);
protected:
private:
	Spi_Hard();
	~Spi_Hard();
	Spi_Hard( const Spi_Hard &c );
	Spi_Hard& operator=( const Spi_Hard &c );

}; //Spi_Hard

#endif //__SPI_HARD_H__
