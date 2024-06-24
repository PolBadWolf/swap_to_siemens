/* 
* Spi.cpp
*
* Created: 12.04.2024 14:46:45
* Author: User
*/


#include "Spi.h"

// default constructor
Spi::Spi() : Spi_soft()
{
} //Spi

// default destructor
Spi::~Spi()
{
} //~Spi

uint8_t	Spi::readByte()
{
#ifdef CONF_SPI_SOFT
	return	Spi_soft::readByte();
#endif // CONF_SPI_SOFT
	return	0;
}

void	Spi::sendByte(uint8_t dat)
{
	Spi_soft::sendByte(dat);
}

uint8_t	Spi::readSendByte(uint8_t datSend)
{
	return	Spi_soft::readSendByte(datSend);
}

void	Spi::setSS(bool	ss)
{
	Spi_soft::setSS(ss);
}
