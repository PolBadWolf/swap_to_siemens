/* 
* StartReady.cpp
*
* Created: 17.10.2024 9:40:23
* Author: User
*/


#include "StartReady.h"
#include "signal_pin.h"

StartReady		*StartReady::obj = NULL;

// default constructor
StartReady::StartReady()
{
} //StartReady

// default destructor
StartReady::~StartReady()
{
} //~StartReady


StartReady*		StartReady::init() //StartReadyTypeMetod mtRead, StartReadyTypeMetod mtSend)
{
	if (obj == 0)		obj = new StartReady;
	obj->stat_st	= ns_pins::transfer_startStop();
	obj->stat_fl	= STARTREADY_STAT_WAIT;
	obj->fStart = 128;
	irqOff();
	// прерывание по изменению
	set_EISR_B.EISC5 = EISR::TwoLevel;
	return	obj;
}

StartReady*		StartReady::getObj()
{
	return	obj;
}

//внешнее прерывание. обработчик.
ISR (INT5_vect)
{
	StartReady::interrupt();
}

void	StartReady::interrupt()
{
	obj->interrupt_int();
}

void	StartReady::interrupt_int()
{
// 	if (fStart > 0)
// 	{
// 		fStart--;
// 		return;
// 	}
	stat_fl		= stat_st	= ns_pins::transfer_startStop();
// 	if (mtRead != 0)	callMetodStartReady(mtRead, stat_st);
// 	if (mtSend != 0)	callMetodStartReady(mtSend, stat_st);
	if (externMetod != 0)	callMetodStartReady(externMetod, stat_st);
}

void	StartReady::resetFlag()
{
	stat_fl = STARTREADY_STAT_WAIT;
}

uint8_t	StartReady::getStatFl()
{
	return	stat_fl;
}

void	StartReady::irqOn(StartReadyTypeMetod externMetod)
{
	fStart = 250;
	this->externMetod = externMetod;
	bit_is_byte(EIMSK).EIMSK_INT5 = 1;
}

void	StartReady::irqOff()
{
	bit_is_byte(EIMSK).EIMSK_INT5 = 0;
}
