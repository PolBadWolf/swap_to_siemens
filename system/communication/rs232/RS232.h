/* 
* RS232.h
*
* Created: 12.04.2024 9:14:43
* Author: User
*/


#ifndef __RS232_H__
#define __RS232_H__

#include "core/core_def.h"
#include "../Serial.h"
#include "core/Fifo.h"

enum rsBaud
{
	baud300     = 300
	,baud600    = 600
	,baud1200   = 1200
	,baud2400   = 2400
	,baud4800   = 4800
	,baud9600   = 9600
	,baud19200  = 19200
	,baud38400  = 38400
	,baud57600  = 57600
	,baud115200 = 115200
	,baud250000 = 250000
};
enum rsParity
{
	DISABLE = 0,
	EVEN = 2,
	ODD = 3
};
enum rsBit
{
	bit5 = 0,
	bit6,
	bit7,
	bit8
};

class RS232	: Serial
{
//variables
public:
static	RS232		*obj;
protected:
	Fifo<uint8_t>	*buffInp;
	Fifo<uint8_t>	*BuffOut;
private:
	uint8_t			mask;

//functions
public:
	static	Serial*		init(rsBaud	baud, rsParity	parity, rsBit	bit, uint8_t	sizeBuffInp, uint8_t	sizeBuffOut);
	void	_interruptSend();
	void	_interruptAccept();
	bool	sendByte(uint8_t	dat);
	bool	readByte(uint8_t	*dat);
protected:
private:
	RS232();
	~RS232();
	RS232( const RS232 &c );
	RS232& operator=( const RS232 &c );

}; //RS232

#endif //__RS232_H__
