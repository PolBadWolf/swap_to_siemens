/* 
* Spi.h
*
* Created: 12.04.2024 14:46:45
* Author: User
*/


#ifndef __Spi_H__
#define __Spi_H__

#include "core/core_def.h"
#include "Spi/Spi_soft.h"

class Spi	: public Spi_soft
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
	Spi();
	~Spi();
protected:
private:
	Spi( const Spi &c );
	Spi& operator=( const Spi &c );

}; //Spi

#endif //__Spi_H__
