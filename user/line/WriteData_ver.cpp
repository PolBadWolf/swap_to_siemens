/* 
* WriteData.cpp
*
* Created: 28.05.2024 16:10:28
* Author: User
*/


#include "WriteData.h"
#include "signal_pin.h"
#include "core/delay.h"
#include "user/mainUser.h"

//#include "core/core_timers.h"

using namespace ns_pins;

//uint16_t	WriteData::f_timer;
#define		__DEBAG__

#ifdef	__DEBAG__
#define		WR_PRE_BUSY_DN		1		// mili sec
#define		WR_PRE_START_UP		1		// micro sec

#define		WR_OUT_SPR_DN		1		// mili sec
#define		WR_OUT_SPR_UP		1		// mili sec
#define		WR_OUT_DATA			1		// micro sec
#define		WR_OUT_STROBE		1		// micro sec

#define		WR_AFT_SPR_UP		1		// mili sec
#define		WR_AFT_START_DN		1		// mili sec
#define		WR_AFT_BUSY_UP		1		// mili sec
#else
#define		WR_PRE_BUSY_DN		500		// mili sec
#define		WR_PRE_START_UP		300		// micro sec

#define		WR_OUT_SPR_DN		59		// mili sec
#define		WR_OUT_SPR_UP		141		// mili sec
#define		WR_OUT_DATA			100		// micro sec
#define		WR_OUT_STROBE		100		// micro sec

#define		WR_AFT_SPR_UP		50		// mili sec
#define		WR_AFT_START_DN		10		// mili sec
#define		WR_AFT_BUSY_UP		50		// mili sec
#endif

WriteData	*WriteData::obj = 0;

// default constructor
WriteData::WriteData()
{
} //WriteData

// default destructor
WriteData::~WriteData()
{
} //~WriteData

// ---------------------------------------------
WriteData*		WriteData::init()
{
	if (obj == 0)	obj = new WriteData;
	// настройка портов
	obj->initPorts();
	//
//	f_timer = Core::timer0->F_TIMER;
	// текущее состояние модуля передачи
	obj->statWork = offline;
	obj->countTikDelay = 0;
	// режим офф
	return	obj;
}

void	WriteData::initPorts()
{
	// =================== data trand ==================
	init_dataEnableOut();
	// =================== spocket =====================
	init_sprocketOut();
	// =================== strobe ======================
	init_strobeOut();
	// =================== dataEnable ==================
	init_dataEnableInp();
	// =================== readyBusy ===================
	init_readyBusyOut();
	// =================== slewInc =====================
	init_slewIncInp();
	// ============ leftRight ======================
	init_leftRightInp();
	// ================== startStop ==================
	init_startStopInp();
	// ================== eot or rhu ==================
	init_eotOrRhuOut();
}

void	WriteData::timerFast_irq()
{
	obj->timerFast();
}

void	WriteData::timerFast()
{
	eventOn		= 1;
	while (eventOn != 0)
	{
		eventOn	= 0;
		switch (statWork)
		{
			case offline:							break;
			case modOff:							break;
			case sendEnd:							break;
			case delay:			mode_delay();		break;
			// ------------------------
			case phaze1_1:		mode_phaze1();	break;
			case phaze2_1:		mode_phaze2_1();	break;
			case phaze2_2:		mode_phaze2_2();	break;
			case phaze2_3:		mode_phaze2_3();	break;
			case phaze3_1:		mode_phaze3_1();	break;
			case phaze3_2:		mode_phaze3_2();	break;
			// ------------------------
			case error:								break;
			case error_delay:						break;
			// ------------------------
			default:
			statWork = error;
			break;
		}
	}
	//
}

void		WriteData::sendOn()
{
	CRITICAL_SECTION
	{
		// проверка режима
		if (statWork == offline)
		{
			// обнуление количества переданных байт
			sendCountByte	= 0;
			countTikDelay	= 0;
			// включение режима передачи **********************
			modeDelay(phaze1_1, WR_PRE_BUSY_DN);
		}
	}
}

void		WriteData::sendOff()
{
	
}

uint8_t		WriteData::getStatusWork()
{
	uint8_t	stat;
	CRITICAL_SECTION
	{
		stat = statWork;
	}
	return	stat;
}

uint16_t	WriteData::getSendCountByte()
{
	uint16_t	count;
	CRITICAL_SECTION
	{
		count = sendCountByte;
	}
	return	count;
}

void	WriteData::modeDelay(StatWork mode, uint16_t	tik)
{
	if (tik == 0)
	{
		statWork = modeAfterWait = mode;
		countTikDelay = tik;
		eventOn = 1;
	} 
	else
	{
		modeAfterWait = mode;
		countTikDelay = tik;
		statWork = delay;
	}
}

// =============================
// ************ modes ***********

void	WriteData::mode_delay()
{
	if (countTikDelay > 0)
	{
		countTikDelay--;
		if (countTikDelay == 0)
		{
			statWork	= modeAfterWait;
			eventOn		= 1;
		}
	}
	else
	{
		statWork	= error_delay;
	}
}

void	WriteData::mode_phaze1()
{
	transfer_readyBusy(0);
	__delay_us(WR_PRE_START_UP);
	transfer_startStop(1);
	modeDelay(phaze2_1, WR_OUT_SPR_DN);
}

void	WriteData::mode_phaze2_1()	// sproket спад
{
	transfer_sprocket(0);
	modeDelay(phaze2_2, WR_OUT_SPR_DN);
}

void	WriteData::mode_phaze2_2()	// вывод данных, строб
{
	// очередной байт
	uint8_t	dat, stat;
	stat = ns_user::flash->fRd_readByte(&dat);
	transfer_data(dat);
	__delay_us(WR_OUT_DATA);
	//импульс строба
	transfer_strobe(1);
	__delay_us(WR_OUT_STROBE);
	transfer_strobe(0);
	// sproket фронт
	transfer_sprocket(1);
	//
	sendCountByte++;
	//
	if (stat == 0)
	{
		// конец передачи
		modeDelay(phaze3_1, WR_AFT_SPR_UP);
	} 
	else
	{
		// формирование площадки sproket
		modeDelay(phaze2_1, WR_OUT_SPR_UP);
	}
}

void	WriteData::mode_phaze2_3()
{
	//modeDelay(phaze2_1, 0);
}

void	WriteData::mode_phaze3_1()
{
	transfer_startStop(0);
	modeDelay(phaze3_2, WR_AFT_START_DN);
}

void	WriteData::mode_phaze3_2()
{
	transfer_readyBusy(1);
	modeDelay(sendEnd, WR_AFT_BUSY_UP);
}


