/* 
* Spi_soft.cpp
*
* Created: 12.04.2024 13:55:46
* Author: User
*/

#include "config_sys.h"

#ifdef CONF_SPI_SOFT

#include "Spi_soft.h"
#include "core/bits.h"
#include <avr/io.h>
#include "core/delay.h"

// ============= INCLUDE ==================
// #define		MOSI_OUT	PORTB
// #define		MOSI_DDR	DDRB
// #define		MOSI_BIT	bit7
// 
// #define		MISO_OUT	PORTD
// #define		MISO_DDR	DDRD
// #define		MISO_INP	PIND
// #define		MISO_BIT	bit5
// 
#define		MOSI_OUT	PORTD
#define		MOSI_DDR	DDRD
#define		MOSI_BIT	bit5

#define		MISO_OUT	PORTB
#define		MISO_DDR	DDRB
#define		MISO_INP	PINB
#define		MISO_BIT	bit7

#define		SCK_OUT		PORTD
#define		SCK_DDR		DDRD
#define		SCK_BIT		bit4

#define		SS_OUT		PORTB
#define		SS_DDR		DDRB
#define		SS_BIT		bit6
// ----------------------------------------

// default constructor
Spi_soft::Spi_soft()
{
	// MOSI
	bit_is_byte(MOSI_OUT).MOSI_BIT = 1;
	bit_is_byte(MOSI_DDR).MOSI_BIT = 1;
	//MISO
	bit_is_byte(MISO_OUT).MISO_BIT = 1;
	bit_is_byte(MISO_DDR).MISO_BIT = 0;
	// SCK
	bit_is_byte(SCK_OUT).SCK_BIT = 0;
	bit_is_byte(SCK_DDR).SCK_BIT = 1;
	// SS
	bit_is_byte(SS_OUT).SS_BIT = 1;
	bit_is_byte(SS_DDR).SS_BIT = 1;

} //Spi_soft

// default destructor
Spi_soft::~Spi_soft()
{
} //~Spi_soft

 uint8_t	Spi_soft::readByte()
 {
	 uint8_t	i, byte = 0;
	 for (i = 0; i < 8; i++)
	 {
		 bit_is_byte(SCK_OUT).SCK_BIT = 1;
		 byte <<= 1;
		 bit_is_byte(byte).bit0 = bit_is_byte(MISO_INP).MISO_BIT;
		 bit_is_byte(SCK_OUT).SCK_BIT = 0;
		 __builtin_avr_delay_cycles(50);
	 }
	 return	byte;
 }
 
 void	Spi_soft::sendByte(uint8_t dat)
 {
	 uint8_t	i;
	 for (i	= 0; i < 8; i++)
	 {
		 bit_is_byte(MOSI_OUT).MOSI_BIT = bit_is_byte(dat).bit7;
		 __builtin_avr_delay_cycles(50);		// возмоно потребуется
		 bit_is_byte(SCK_OUT).SCK_BIT = 1;
		 dat <<= 1;
		 bit_is_byte(SCK_OUT).SCK_BIT = 0;
		 __builtin_avr_delay_cycles(50);
	 }
 }

uint8_t	Spi_soft::readSendByte(uint8_t datSend)
{
	uint8_t	i, datRead = 0;
	for (i = 0; i < 8; i++)
	{
		 bit_is_byte(MOSI_OUT).MOSI_BIT = bit_is_byte(datSend).bit7;
		 bit_is_byte(SCK_OUT).SCK_BIT = 1;
		 datSend <<= 1;
		 datRead <<= 1;
		 bit_is_byte(datRead).bit0 = bit_is_byte(MISO_INP).MISO_BIT;
		 bit_is_byte(SCK_OUT).SCK_BIT = 0;
		 __builtin_avr_delay_cycles(2);
	}
	return	datRead;
}

void	Spi_soft::setSS(bool	ss)
{
	bit_is_byte(SS_OUT).SS_BIT = ss;
}

#endif // CONF_SPI_SOFT
