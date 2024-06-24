/* 
* Spi_soft.h
*
* Created: 12.04.2024 13:55:46
* Author: User
*/


#ifndef __SPI_SOFT_H__
#define __SPI_SOFT_H__

#include "core/core_def.h"
// #include "../SPI.h"


class Spi_soft
{
//variables
public:
	uint8_t	readByte();
 	void	sendByte(uint8_t dat);
	uint8_t	readSendByte(uint8_t datSend);
	void	setSS(bool	ss);
protected:
private:

//functions
public:
	Spi_soft();
	~Spi_soft();
protected:
private:
	Spi_soft( const Spi_soft &c );
	Spi_soft& operator=( const Spi_soft &c );

}; //Spi_soft

#endif //__SPI_SOFT_H__
