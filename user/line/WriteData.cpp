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
#include "user/line/Simulyation.h"
#include "user/var.h"
#include <avr/pgmspace.h>

#include "core/core_timers.h"
// #include "user/mainUserCore.h"

#include "user/line/StartReady.h"

#include "indication/Lcd_hard.h"

#define		TIMER_FEQ	timer0_FEQ

using namespace ns_pins;

// #define		__DEBAG__

#ifdef	__DEBAG__
#define		WR_PRE_BUSY_DN		1		// / 2	mili sec
#define		WR_PRE_START_UP		1		// micro sec

#define		WR_OUT_SPR_DN		1		// 2		// 2	 mili sec
#define		WR_OUT_SPR_UP		2		// 4		// 2	 mili sec
#define		WR_OUT_DATA			1		// micro sec
#define		WR_OUT_STROBE		1		// micro sec

#define		WR_AFT_SPR_UP		1		// / 2	mili sec
#define		WR_AFT_START_DN		1		// / 2	mili sec
#define		WR_AFT_BUSY_UP		1		// / 2	mili sec
#error lock tree
// -------------------------------------------------------------------------------------------------------------------------
#else

// коэффицент в тиках (точность кратна WRITEDATA_TimerFast), (uint8_t)(((double)mSec) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))

// 500 mSec
// #define		WR_PRE_BUSY_DN		(uint8_t)(((double)500.00) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))
#define		WR_PRE_BUSY_DN		ns_var::wr_Pre_Busy_Dn_k

// #define		WR_PRE_START_UP		300		// ????

// #define		WR_OUT_SPR_DN		(uint8_t)(((double)1.25) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))
#define		WR_OUT_SPR_DN		ns_var::wr_Out_Spr_Dn_k
// #define		WR_OUT_SPR_UP		(uint8_t)(((double)2.00) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))
#define		WR_OUT_SPR_UP		ns_var::wr_Out_Spr_Up_k

#define		WR_OUT_DATA			100		// micro sec
#define		WR_OUT_STROBE		100		// micro sec

// #define		WR_AFT_SPR_UP		(uint8_t)(((double)2.00) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))
// #define		WR_AFT_START_DN		(uint8_t)(((double)1.00) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))
//#define		WR_AFT_BUSY_UP		(uint8_t)(((double)2.00) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))
#define		WR_AFT_BUSY_UP		ns_var::wr_Pre_Busy_Up_k
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
uint32_t xx2;
// ---------------------------------------------
WriteData*		WriteData::init()
{
	if (obj == 0)	obj = new WriteData;
	// настройка портов
	obj->initPorts();
	//
	obj->fl_reset = 0;
	// текущее состояние модуля передачи
	obj->statWork = offline;
	obj->countTikDelay = 0;
	// настройка коэфицентов
	obj->read_wr_kof();
	
	// режим офф
	return	obj;
}

WriteData*		WriteData::getObj()
{
	return	obj;
}

void	WriteData::initPorts()
{
	// =================== data trand ==================
	init_dataOut();
	// =================== sprocket =====================
	init_sprocketOut();
	// =================== strobe ======================
	init_strobeOut();
	// =================== readyBusy ===================
	init_readyBusyOut();
	// ================== startStop ==================
	if (ns_var::simulOn != 0)	init_startStopOut();
	else						init_startStopInp();
	// ================== eot or rhu ==================
	init_eotOrRhuOut();
	// ================== left / right ==================
	init_leftRightInp();
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
			case offline:									break;
			case modOff:									break;
			case sendEnd:									break;
			case delay:				mode_delay();			break;
			// ------------------------
			case stat_init:			mode_stat_init();		break;	// исходное состояние
			case stat_sprocket_dn:	mode_sprocket_dn();		break;	// sprocket спад
			case stat_sendByte:		mode_sendByte();		break;	// передача байта данных, проверка окончания передачи
			case phaze3_1:			mode_phaze3_1();		break;
			case phaze3_2:			mode_phaze3_2();		break;
			// ------------------------
			case error:										break;
			case error_delay:								break;
			// ------------------------
			default:
			statWork = error;
			break;
		}
		// -----------
		if (fl_reset)
		{
			fl_reset = 0;
			eventOn = 0;
			statWork = offline;
		}
	}
	//
}

	static	uint16_t	x1	= 0;
	static	uint16_t	x2	= 0;

void		WriteData::sendOn()
{
	CRITICAL_SECTION
	{
		x1 = 0;
		x2 = 0;
		// проверка режима
		if (statWork == offline)
		{
			// обнуление количества переданных байт
			sendCountByte	= 0;
			countTikDelay	= 0;
			error_sim		= 0;
			sim_adr			= ns_simul::flsh_avr_startAdr;
			if ((ns_var::s_prog != 0) || (ns_var::simulOn != 0)) // системная программа или симуляция
			{
				startHeaderCount = 0;
			} 
			else
			{	// заголовое из "0" перед передачей
				startHeaderCount = 50;
			}
			// длина хвоста из "0" после окончания передачи
			postSend_var = postSend_const;
			// ---------------
			startStopDelaySimulCount = 1.5 * WRITEDATA_TimerFast;			// задержка сигнала Start/stop при симуляции на 1.5 секунды
			// ---------------
			ns_var::error_parity = 0;
			// исходное состояние перед передачей :
			// отключение готовности до сигнала старт
			transfer_readyBusy(1);
			// спрокет и строб в начальное состояние
			transfer_sprocket(1);
			transfer_strobe(0);
			// включение режима передачи **********************
			// время установки исходного состояния перед передачей; stat_init на следущем цикле
			modeDelay(stat_init, WR_PRE_BUSY_DN);
			// включение прерывания по сигналу Start/Stop
			{
				StartReady	*objX = StartReady::getObj();
				if (objX != 0)
				{
					objX->irqOn(switchStart);
				}
			}
		}
	}
}


const uint8_t	pos_switchStart_const	= scr->SetPosition(6, 1);
void		WriteData::switchStart(uint8_t stat)
{
	if (stat == STARTREADY_STAT_OFF)	x1++;

	if (stat == STARTREADY_STAT_ON)		x2++;

	uint8_t	pos = pos_switchStart_const;
	
	scr->DigitZ(&pos, 2, x1);
// 	scr->PutChar(&pos, ' ');
	pos++;
	scr->DigitZ(&pos, 2, x2);

// 	WriteData	*obj = WriteData::getObj();
// 	if (stat == STARTREADY_STAT_OFF)
// 	{
//  		transfer_readyBusy(1);
// 		 __delay_ms(5000);
// 		CRITICAL_SECTION
// 		{
// 			scr->Clear();
// 			scr->String_P( PSTR("прерывание") );
// 			for (;;)
// 			{
// 				((Lcd_hard *)scr)->Interrupt_static();
// 				__delay_ms(1);
// 			}
// 		}
// 	}
}

void		WriteData::sendOff()
{
	StartReady::irqOff();
	transfer_readyBusy(1);
	CRITICAL_SECTION
	{	// сброс работы модуля
		fl_reset = 1;
	}
	__delay_ms(5);
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

void	WriteData::mode_stat_init()		// начальное состояние
{
	// чтение сигнала "start"
	uint8_t	startStop	= transfer_startStop();
	uint8_t simulOn		= ns_var::simulOn;
	if (simulOn != 0)
	{
		if (startStopDelaySimulCount > 0)
		{
			startStopDelaySimulCount--;
			return;
		}
		transfer_startStop(1);
	}
	// ожидание начала передачи
	if (startStop == 0)		return;
	// ------------
	transfer_readyBusy(0);
// 	__delay_us(WR_PRE_START_UP); // ???
	
// 	modeDelay(stat_sendByte, WR_OUT_SPR_DN);
	modeDelay(stat_sprocket_dn, WR_OUT_SPR_DN);
}

void	WriteData::mode_phaze_send_strb(uint8_t dat)
{
	transfer_data(dat);
	__delay_us(WR_OUT_DATA);
	// импульс строба
	transfer_strobe(1);
	// sproket фронт
	transfer_sprocket(1);
	__delay_us(WR_OUT_STROBE);
	// завершение импульса строба
	transfer_strobe(0);
}

uint8_t	WriteData::mode_chkFlagSend()
{
	uint8_t		flag_send		= 1;
/*
	uint8_t		st_leftRight	= ns_pins::transfer_leftRight();

	// контроль сигнала lift/right отключен
	if (ns_var::leftRight_stat == 0)
	{
		flag_send = 1;
	}
	
	if ( (ns_var::leftRight_stat == 1) && (st_leftRight == 0) )
	{
		flag_send = 1;
	}

	if ( (ns_var::leftRight_stat == 2) && (st_leftRight != 0) )
	{
		flag_send = 1;
	}
*/
	// ------------------
	return	flag_send;
}

void	WriteData::mode_sprocket_dn()	// sproket спад
{
	transfer_sprocket(0);
	modeDelay(stat_sendByte, WR_OUT_SPR_DN);
	// при симуляции
	if (ns_var::simulOn != 0)
	{
		uint16_t l = ns_user::flash->get_rd_lenght();
		if (l == 200)
		{
			ns_pins::transfer_startStop(0);
		}
	}
}

void	WriteData::mode_sendByte()	// вывод данных, строб
{
	uint8_t		flag_send		= mode_chkFlagSend();
	// ------------------------------------------------------------
	// очередной байт
	uint8_t	dat, stat;
	
	// разрешение передачи
	if (flag_send != 0)
	{
		if ((ns_var::s_prog != 0) || (ns_var::simulOn != 0))	// системная программа или симуляция
		{
			stat = ns_user::flash->fRd_readByte(&dat);
		}
		else
		{
			if (startHeaderCount > 0)
			{
				stat = 1;
				dat  = 0;
			}
			else
			{
				stat = ns_user::flash->fRd_readByte(&dat);
				dat = odd_plus_7bit(dat);
			}
		}
		uint8_t								fl_end = 0;
		if (stat == 0)						fl_end = 1;
		if (transfer_startStop() == 0)		fl_end = 1;
		//
		if (fl_end == 0)	postSend_var = postSend_const;
		else
		{
			if (postSend_var > 0)	postSend_var--;
		}
		//
		// отправка байта
		mode_phaze_send_strb(dat);
		// -----------------------
		// проверка переданных данных
		if (ns_var::simulOn != 0)
		{
			{
				uint8_t dat_chk;
				#ifdef	__ADR_TO_DATA
				dat_chk = word_to_byte(sim_adr).Low;
				#else
				dat_chk = pgm_read_byte(sim_adr);
				#endif
				if (dat != dat_chk)
				{
					if (error_sim == 0)
					{
						error_sim_adr = sim_adr;
						error_sim_dat = dat_chk;
					}
					error_sim = 1;
				}
				sim_adr++;
			}
		}
		// --------------------------------
		// счетчик переданных байтов ( без заголовок )
		if (startHeaderCount > 0)	startHeaderCount --;
		else						sendCountByte++;
	}

	// ---------------------------------
	// завершение передачи
	/*if (
	(stat == 0)
	|| ((transfer_startStop() == 0)	)
	)*/
	if (postSend_var == 0)
	{
		// конец передачи
		modeDelay(sendEnd, WR_AFT_BUSY_UP);
		StartReady::irqOff();
		transfer_readyBusy(1);
		return;
	}

	// формирование площадки sproket , если разрешена работа
	if (flag_send != 0)
	{
		modeDelay(stat_sprocket_dn, WR_OUT_SPR_UP);
	}
}


void	WriteData::mode_phaze3_1()
{
/*
	uint8_t	flag = mode_chkFlagSend();
	if (flag != 0)
	{
		mode_phaze_send_strb(0);
	}
	modeDelay(phaze3_2, WR_AFT_START_DN);
*/
}

void	WriteData::mode_phaze3_2()
{
/*
	if (transfer_startStop() == 0)
	{
		StartReady::irqOff();
		transfer_readyBusy(1);
// 		modeDelay(sendEnd, WR_AFT_BUSY_UP);
		modeDelay(sendEnd, 9000);
		return;
	}
	//
	if (postSend_var > 0)
	{
		postSend_var--;
		modeDelay(phaze3_1, WR_OUT_SPR_UP);
		transfer_sprocket(0);
	} 
	else
	{
		transfer_readyBusy(1);
		// end send
		modeDelay(sendEnd, WR_AFT_BUSY_UP);
	}
	//
*/
}

// #define		WR_OUT_SPR_DN		(uint8_t)(((double)1.25) * ((double)WRITEDATA_TimerFast) / ((double)1000.0))
uint16_t	WriteData::convFloatToTik(double ms)
{
	uint16_t	tik = 0;
	tik = ms / unitTimerTik;
	return	tik;
}

double		WriteData::convTikToFloat(uint16_t tik)
{
	double	ms = 0;
	ms = tik * unitTimerTik;
	return ms;
}

void	WriteData::read_wr_kof()
{
	// wr_Out_Spr_Dn
	ns_var::wr_Out_Spr_Dn_k		= convFloatToTik(eeprom_read_float(&ns_var::wr_Out_Spr_Dn_e));
	
	// wr_Out_Spr_Up
	ns_var::wr_Out_Spr_Up_k		= convFloatToTik(eeprom_read_float(&ns_var::wr_Out_Spr_Up_e));

	// wr_Pre_Busy_Dn
	ns_var::wr_Pre_Busy_Dn_k	= convFloatToTik(eeprom_read_float(&ns_var::wr_Pre_Busy_Dn_e));

	// wr_Pre_Busy_Up
	ns_var::wr_Pre_Busy_Up_k	= convFloatToTik(eeprom_read_float(&ns_var::wr_Pre_Busy_Up_e));
	
}

