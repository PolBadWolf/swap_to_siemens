/* 
* ReadData.cpp
*
* Created: 16.05.2024 15:06:41
* Author: User
*/


#include "ReadData.h"
#include "core/core.h"
#include "signal_pin.h"
#include "user/mainUser.h"
#include <avr/pgmspace.h>
#include "user/var.h"
#include "user/line/Simulyation.h"

//#define BUSY_ON

ReadData	*ReadData::obj = 0;

// default constructor
ReadData::ReadData()
{
} //ReadData

// default destructor
ReadData::~ReadData()
{
} //~ReadData

// ===================================
ReadData*	ReadData::init()
{
	if (obj == 0)	obj = new ReadData;
	// init
	CRITICAL_SECTION
	{
		obj->blockRead	= 1;
		obj->statWork	= Offline;
	}
	//
	return	obj;
}

void	ReadData::timerFast()
{
	if (blockRead != 0)		return;
	if (ns_var::simulOn)	ns_simul::read_cycle();
	switch (statWork)
	{
	case Offline:
	break;
	case	Wait_InitialState:			// ожидание исходного состояния для начала приема
		int_Wait_InitialState();
	break;
	case	Wait_StartRead:				// ожидание сигнала старт приема
		int_Wait_StartRead();
	break;
	case	Wait_ByteCompletion:		// ожидание завершение приема байта
		int_Wait_ByteCompletion();
	break;
	case	Wait_ByteRead:				// ожидание  приема байта
		int_Wait_ByteRead();
	break;
	default:
	break;
	}
}

void	ReadData::timerFast_irq()
{
	obj->timerFast();
}

uint8_t	ReadData::readOn(uint32_t freeSize)
{
	uint8_t	stat;
	CRITICAL_SECTION
	{
		stat =  statWork;
	}
	if (stat != Offline)	return	1;
	CRITICAL_SECTION
	{
		initPorts();
		this->wr_freeSize = freeSize;
		statWork = Wait_InitialState;
		blockRead	= 0;
		wr_overSize	= 0;
	}
	return 0;
}

uint8_t ReadData::readOff()
{
	CRITICAL_SECTION
	{
		statWork = Offline;
		blockRead	= 1;
	}
	return	0;
}

void	ReadData::initPorts()
{
	if (ns_var::simulOn)
	{
		ns_pins::init_dataOut();
		ns_pins::init_sprocketOut();
		ns_pins::init_strobeInp();
		ns_pins::init_dataEnableInp();
		ns_pins::init_readyBusyInp();
		ns_pins::init_slewIncOut();
		ns_pins::init_leftRightInp();
		ns_pins::init_startStopOut();
		ns_pins::init_eotOrRhuInp();
	} 
	else
	{
		// настройка портов на прием
		ns_pins::init_dataInp();
		ns_pins::init_sprocketInp();
		ns_pins::init_strobeInp();
		ns_pins::init_dataEnableInp();
		ns_pins::init_readyBusyInp();
		ns_pins::init_slewIncInp();
		ns_pins::init_leftRightInp();
		ns_pins::init_startStopInp();
		ns_pins::init_eotOrRhuInp();
	}
}

void	ReadData::int_Wait_InitialState()
{
// 	if (
// 	(ns_pins::transfer_slewInc() != 0)
	// 	&&	(ns_pins::transfer_startStop() == 0)
	//	&&	(ns_pins::transfer_spocket() != 0)
// 	)
	{
		ns_var::error_parity = 0;
		ns_var::fl_puskRead = 0;
		statWork = Wait_StartRead;
	}
}

void	ReadData::int_Wait_StartRead()
{
	if (
// 		(ns_pins::transfer_slewInc() != 0)		&&
		(ns_pins::transfer_startStop() != 0)	&&
		(ns_pins::transfer_sprocket() == 0)
	)
	{
		statWork = Wait_ByteRead;
	}
}

void	ReadData::int_Wait_ByteCompletion()
{
	// окончание сигнала "старт/стоп"
	if (ns_pins::transfer_startStop() == 0)
	{
		blockRead	= 1;
		statWork	= EndRead;
		return;
	}
	// ожидание спада строба
	if (ns_pins::transfer_sprocket() != 0)	return;
/*
#ifdef BUSY_ON
	if (ns_pins::transfer_readyBusy() != 0)	return;	// нет готовности: ждать готовность или сообщить на верх ?
#endif // BUSY_ON
*/
	statWork  = Wait_ByteRead;
}

uint8_t	ReadData::checkErrorParity(uint8_t dat)
{
	uint8_t stat = 0;
	uint8_t bc = bit_is_byte(dat).bit7;
	uint8_t cc = 0;
	for (uint8_t i = 0; i < 7; i++)
	{
		if ((dat & 1) != 0)	cc++;
		dat >>= 1;
	}
	stat = bc ^ (cc & 1);
	return	stat;
}

void	ReadData::int_Wait_ByteRead()
{
	if (ns_pins::transfer_sprocket() != 0)
	{
		uint8_t dat = ns_pins::transfer_data();
		if (ns_var::fl_puskRead)
		{
			serialDataSend(dat);
			ns_var::error_parity |= checkErrorParity(dat);
		} 
		else
		{
			if ((ns_var::s_prog != 0) || (ns_var::simulOn != 0))
			{	//системная программа
				if (dat != 0)
				{
					ns_var::fl_puskRead = 1;
					serialDataSend(0);
					serialDataSend(dat);
				}
			}
			else
			{	// короткая рабочая программа
				if ((dat & 0x7f) == '%')
				{
					ns_var::fl_puskRead = 1;
					serialDataSend(0);
					serialDataSend(dat);
				}
			}
		}
		statWork = Wait_ByteCompletion;
	}
	if (wr_freeSize == 0)
	{
		blockRead	= 1;
		wr_overSize	= 1;
		statWork	= EndRead;
	}
	if (ns_pins::transfer_startStop() == 0)
	{
		blockRead	= 1;
		statWork	= EndRead;
	}
}

void	ReadData::serialDataSend(uint8_t dat)
{
	ns_user::flash->fWr_dataSend(dat);
	wr_freeSize--;
}

uint16_t	ReadData::getWrFreeSize()
{
	uint16_t len;
	CRITICAL_SECTION
	{
		len = wr_freeSize;
	}
	return	len;
}

uint8_t		ReadData::getWrOverSize()
{
	uint8_t	stat;
	CRITICAL_SECTION
	{
		stat = wr_overSize;
	}
	return	stat;
}


uint8_t	ReadData::getStatWork()
{
	return	statWork;
}

void	ReadData::cancel()
{
	CRITICAL_SECTION
	{
		blockRead = 1;
		statWork = Cancel;
	}
}

void	ReadData::reset()
{
	if ((statWork == Error)
	&&	(statWork == Cancel))
	{
		statWork = Offline;
	}
}


