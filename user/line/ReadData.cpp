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

void	ReadData::setStatWork(StatWork statusWork)
{
	statWork		= statusWork;
	countSafeDelay	= 0;
	countSafeIntegr	= 0;
}

// ===================================
ReadData*	ReadData::init()
{
	if (obj == 0)	obj = new ReadData;
	// init
	CRITICAL_SECTION
	{
		obj->blockRead	= 1;
		obj->setStatWork(Offline);
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
		this->wr_freeSize = freeSize - 4;
		setStatWork(Wait_InitialState);
		blockRead	= 0;
		wr_overSize	= 0;
		ns_var::waitEndCount = 0;
		wr_lenght = 0;
	}
	return 0;
}

uint8_t ReadData::readOff()
{
	CRITICAL_SECTION
	{
		setStatWork(Offline);
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
		ns_pins::init_startStopOut();
		ns_pins::init_eotOrRhuInp();
	} 
	else
	{
		// настройка портов на прием
		ns_pins::init_dataInp();
		ns_pins::init_sprocketInp();
		ns_pins::init_strobeInp();
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
		setStatWork(Wait_StartRead);
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
		setStatWork(Wait_ByteRead);
	}
}

void	ReadData::int_Wait_ByteCompletion()
{
	// окончание сигнала "старт/стоп"
	if (ns_pins::transfer_startStop() == 0)
	{
		blockRead	= 1;
		setStatWork(EndRead);
		return;
	}
	if (wr_freeSize == 0)
	{
		blockRead	= 1;
		wr_overSize	= 1;
		setStatWork(EndRead);
	}
	// ожидание спада строба
	/*
	if (ns_pins::transfer_sprocket() != 0)	return;
	setStatWork(Wait_ByteRead);
	*/
	if (ns_pins::transfer_sprocket() == 0)
	{
		if (countSafeIntegr < ns_var::safeDelay_minINT)		{	countSafeIntegr++;	}
		if (countSafeDelay  < ns_var::safeDelay_minSD )		{	countSafeDelay++;	}
		if (countSafeDelay >= ns_var::safeDelay_minINT)
		{
			setStatWork(Wait_ByteRead);
		}
	}
	else
	{
		if (countSafeIntegr  > 0)		{	countSafeIntegr--;	}
		if (countSafeIntegr == 0)		{	countSafeDelay = 0;	}
		if (countSafeDelay   > 0)		{	countSafeDelay--;	}
	}
}

uint8_t	ReadData::checkErrorParity(uint8_t dat)
{
	uint8_t stat = 0;
	uint8_t bc = bit_is_byte(dat).bit7;
	uint8_t cc = 0;
	cc = odd_from_7bit(dat);
	stat = bc ^ (cc & 1);
	return	stat;
}

void	ReadData::transfer_readByte()
{
	if ((ns_var::s_prog != 0) || (ns_var::simulOn != 0))	// системная программа или симуляция
	{
		serialDataSend(datDelay);
		setStatWork(Wait_ByteCompletion);
		wr_lenght++;
		return;
	}
	uint8_t dat = datDelay & 0x7f;
	if (ns_var::fl_puskRead != 0)	// рабочая программа
	{
		ns_var::error_parity |= checkErrorParity(datDelay);
		serialDataSend(dat);
		setStatWork(Wait_ByteCompletion);
		wr_lenght++;
		switch (ns_var::waitEndCount)
		{
			case 0:
			if (dat == 0x0d)	ns_var::waitEndCount = 1;
			else				ns_var::waitEndCount = 0;
			break;
			case 1:
			if (dat == 0x0a)	ns_var::waitEndCount = 2;
			else				ns_var::waitEndCount = 0;
			break;
			case 2:
			if (dat == 0x0d)	ns_var::waitEndCount = 3;
			else				ns_var::waitEndCount = 0;
			break;
			case 3:
			if (dat == 0x0a)	ns_var::waitEndCount = 4;
			else				ns_var::waitEndCount = 0;
			break;
			case 4:
			if (dat == 0x0d)	ns_var::waitEndCount = 3;
			else				ns_var::waitEndCount = 0;
			break;
			default:
			ns_var::waitEndCount = 0;
			break;
		}
		return;
	}
	// ожидание рабочей программы
	if (dat == '%')
	{
		ns_var::fl_puskRead = 1;
		serialDataSend(dat);
	}
	setStatWork(Wait_ByteCompletion);
	return;
}

void	ReadData::int_Wait_ByteRead()
{
	/*
	if (wr_freeSize == 0)
	{
		blockRead	= 1;
		wr_overSize	= 1;
		setStatWork(EndRead);
	}
	if (ns_pins::transfer_startStop() == 0)
	{
		blockRead	= 1;
		setStatWork(EndRead);
	}
	*/
	//
	if (ns_pins::transfer_sprocket() != 0)
	{
		// integration
		if (countSafeIntegr < ns_var::safeDelay_plsINT)		{	countSafeIntegr++;	}
		// счетчик защитного интервала
		if (countSafeDelay  < ns_var::safeDelay_plsSD)		{	countSafeDelay++;	}
		// фиксирование данных
		if (
			(countSafeDelay == ns_var::safeDelay_plsFD)	||
			(ns_var::safeDelay_plsFD == 0) )				{	datDelay = ns_pins::transfer_data();	}
		// обработка байта после защитного интервала
		if (countSafeDelay >= ns_var::safeDelay_plsSD)		{	transfer_readByte();	}
	}
	else
	{
		if (countSafeIntegr > 0)		{	countSafeIntegr--;	}
		if (countSafeIntegr == 0)		{	countSafeDelay = 0;	}
		if (countSafeDelay > 0)			{	countSafeDelay--;	}
	}
	
	
	/*
	//
	if (ns_pins::transfer_sprocket() != 0)
	{
		uint8_t dat = ns_pins::transfer_data();
		if ((ns_var::s_prog != 0) || (ns_var::simulOn != 0))	// системная программа или симуляция
		{
			serialDataSend(dat);
			setStatWork(Wait_ByteCompletion);
			wr_lenght++;
			return;
		}
		if (ns_var::fl_puskRead != 0)	// рабочая программа
		{
			ns_var::error_parity |= checkErrorParity(dat);
			uint8_t dt = dat & 0x7f;
			serialDataSend(dt);
			setStatWork(Wait_ByteCompletion);
			wr_lenght++;
			switch (ns_var::waitEndCount)
			{
				case 0:
					if (dt == 0x0d)		ns_var::waitEndCount = 1;
					else				ns_var::waitEndCount = 0;
					break;
				case 1:
					if (dt == 0x0a)		ns_var::waitEndCount = 2;
					else				ns_var::waitEndCount = 0;
					break;
				case 2:
					if (dt == 0x0d)		ns_var::waitEndCount = 3;
					else				ns_var::waitEndCount = 0;
					break;
				case 3:
					if (dt == 0x0a)		ns_var::waitEndCount = 4;
					else				ns_var::waitEndCount = 0;
					break;
				case 4:
					if (dt == 0x0d)		ns_var::waitEndCount = 3;
					else				ns_var::waitEndCount = 0;
					break;
				default:
				ns_var::waitEndCount = 0;
			}
			return;
		}
		dat &= 0x7f;
		if (dat == '%')	// ожидание начала рабочей программы
		{
			ns_var::fl_puskRead = 1;
			serialDataSend(dat);
		}
		setStatWork(Wait_ByteCompletion);
		return;
	}
	*/
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
		setStatWork(Cancel);
	}
}

void	ReadData::reset()
{
	if ((statWork == Error)
	&&	(statWork == Cancel))
	{
		setStatWork(Offline);
	}
}


