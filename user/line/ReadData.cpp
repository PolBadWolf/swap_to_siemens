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
#ifdef		__DEBUG_SIMULYATION
	if (ns_var::simRead_count_sproket <= ___SIM_READ_MAX_SPROKET)	ns_var::simRead_count_sproket++;
#endif
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

uint8_t	ReadData::readOn()
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
		statWork = Wait_InitialState;
		blockRead	= 0;
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
{	// настройка портов на прием
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

void	ReadData::int_Wait_InitialState()
{
#ifndef		__DEBUG_SIMULYATION
	if (
	(ns_pins::transfer_slewInc() != 0)
	// 	&&	(ns_pins::transfer_startStop() == 0)
	//	&&	(ns_pins::transfer_spocket() != 0)
	)
#else
	ns_var::simRead_adr				= 0x40;
	ns_var::simRead_count_byte		= 1000;
	ns_var::simRead_count_sproket	= ___SIM_READ_MAX_SPROKET;
#endif
	{
		statWork = Wait_StartRead;
	}
}

void	ReadData::int_Wait_StartRead()
{
#ifndef		__DEBUG_SIMULYATION
	if (
	(ns_pins::transfer_slewInc() != 0)
	&&	(ns_pins::transfer_startStop() != 0)
	&&	(ns_pins::transfer_sprocket() == 0)
	)
#endif
	{
		statWork = Wait_ByteRead;
	}
}

void	ReadData::int_Wait_ByteCompletion()
{
	// окончание сигнала "старт/стоп"
#ifndef			__DEBUG_SIMULYATION
	if (ns_pins::transfer_startStop() == 0)
#else
	if (ns_var::simRead_count_byte == 0)
#endif
	{
		blockRead	= 1;
		statWork	= EndRead;
		return;
	}
	// ожидание спада строба
#ifndef			__DEBUG_SIMULYATION
	if (ns_pins::transfer_sprocket() != 0)	return;
#else
	ns_var::simRead_count_sproket = 0;
#endif
/*
#ifdef BUSY_ON
	if (ns_pins::transfer_readyBusy() != 0)	return;	// нет готовности: ждать готовность или сообщить на верх ?
#endif // BUSY_ON
*/
	statWork  = Wait_ByteRead;
}

void	ReadData::int_Wait_ByteRead()
{
#ifndef			__DEBUG_SIMULYATION
	if (ns_pins::transfer_sprocket() != 0)
	{
		uint8_t dat = ns_pins::transfer_data();
#else
	if (ns_var::simRead_count_sproket >= ___SIM_READ_MAX_SPROKET)
	{
		uint8_t dat = pgm_read_byte(ns_var::simRead_adr);
		ns_var::simRead_adr++;
		ns_var::simRead_count_byte--;
#endif
		ns_user::flash->fWr_dataSend(dat);
		statWork = Wait_ByteCompletion;
	}
#ifndef			__DEBUG_SIMULYATION
	if (ns_pins::transfer_startStop() == 0)
	{
		blockRead	= 1;
		statWork	= EndRead;
	}
#endif
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


