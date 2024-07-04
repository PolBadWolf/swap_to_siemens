/* 
* core.cpp
*
* Created: 10.04.2024 11:34:09
* Author: User
*/

#pragma GCC optimize("O0")

#include "core.h"
#include "core/core_def.h"
#include "core/core_timers.h"
#include "core/TaskQueueUnit.h"
#include "core/delay.h"
#include "timers/Timer0.h"
#include "timers/Timer2.h"
#include "user/mainUser.h"


namespace	Core
{
	void	init();
	void	interrupt0();
	void	interrupt2();
	void	idleCycle();
}


// system var
namespace	Core
{
	extern	TaskQueueUnit		taskQueueHigh[];
	extern	uint8_t				taskQueueHigh_size;
	
	extern	TaskQueueUnit		taskQueueMidle[];
	extern	uint8_t				taskQueueMidle_size;
	
	extern	TaskQueueUnit		taskQueueIdle[];
	extern	uint8_t				taskQueueIdle_size;
	extern	uint8_t				taskQueueIdle_cur;
	
#ifdef CONF_WATHDOG
	extern	bool				wathDogStart;
#endif // CONF_WATHDOG
}

// system obj
namespace	Core
{
	Timer0		*timer0;
	Timer2		*timer2;
}

// ============ INCLUDE  ===============================
#ifdef CONF_LCD
	#include "indication/Lcd_hard.h"
#endif // CONF_LCD

#ifdef CONF_KEY4
	#include "keyboard/Key4.h"
#endif // CONF_KEY4

#ifdef CONF_WATHDOG
	#include "timers/watchdog.h"
	bool		Core::wathDogStart;
#endif // CONF_WATHDOG

#ifdef CONF_RS232
	#include "communication/rs232/RS232.h"
#endif // CONF_RS232

#ifdef CONF_SPI_SOFT
	#include "communication/spi/Spi_soft.h"
#endif // CONF_SPI_SOFT

#ifdef CONF_MENU
#include "menu/menu.h"
#endif // CONF_MENU

#include "user/mainUserCore.h"
// ====================================================
#ifdef CONF_LCD
	Lcd			*scr;
	Lcd_hard	*scrHard;
#endif // CONF_LCD

#ifdef CONF_KEY4
	Key			*key;
#endif // CONF_KEY4

#ifdef CONF_RS232
	Serial		*serial;
#endif // CONF_RS232

#ifdef CONF_SPI_SOFT
	Spi			*spi;
#endif // CONF_SPI_SOFT
// ====================================================
// ====================================================

//  ************** HIGH ***************************
TaskQueueUnit	Core::taskQueueHigh[] = {
	TaskQueueUnit::crTask(delay_irq, 1 * ((double)timer0_FEQ / 1000.0), 0),
// 	TaskQueueUnit::crTask(spaceCountFix, (uint8_t)(250 * ((double)timer0_FEQ / 1000.0)), 0),

#include "user/system/taskUserHigh.h"
};

//  ************** MIDLE ***************************
TaskQueueUnit	Core::taskQueueMidle[] = {

#ifdef CONF_LCD
TaskQueueUnit::crTask(Lcd_hard::Interrupt_static, 1 * ((double)timer2_FEQ / 1000.0), 0),
#endif // CONF_LCD

#ifdef CONF_KEY4
TaskQueueUnit::crTask(Key4::interrupt, 1 * ((double)timer2_FEQ / 1000.0), 0),
#endif // CONF_KEY4

#ifdef CONF_MENU
TaskQueueUnit::crTask(ns_menu::interrupt, (uint8_t)(1 * ((double)timer2_FEQ / 1000.0)), 0),
#endif // CONF_MENU

#include "user/system/taskUserMidle.h"
};

//  ************** IDLE ***************************
TaskQueueUnit	Core::taskQueueIdle[] = {

#ifdef CONF_MENU
TaskQueueUnit::crTask(ns_menu::mainCycle, (uint8_t)(10 * ((double)timer0_FEQ / 1000.0)), 0),
#endif // CONF_MENU

#include "user/system/taskUserIdle.h"
	
// 	TaskQueueUnit::crTask(spacePrint, (uint8_t)(250 * ((double)timer0_FEQ / 1000.0)), 0),
};
// ------------------------------------------------

uint8_t		Core::taskQueueHigh_size;
uint8_t		Core::taskQueueMidle_size;
uint8_t		Core::taskQueueIdle_size;
uint8_t		Core::taskQueueIdle_cur = 0;

int a;

void	Core::init()
{
	// настройка таймера
	timer0 = new Timer0(timer0_FEQ);
	timer2 = new Timer2(timer2_FEQ);
	
	taskQueueHigh_size	= sizeof(taskQueueHigh)		/ sizeof(TaskQueueUnit);
	taskQueueMidle_size	= sizeof(taskQueueMidle)	/ sizeof(TaskQueueUnit);
	taskQueueIdle_size	= sizeof(taskQueueIdle)		/ sizeof(TaskQueueUnit);
	
#ifdef CONF_LCD
	scrHard = new Lcd_hard;
	scr		= (Lcd *)scrHard;
	#ifdef CONF_LCD_24
		scrHard->Init(24);
		#else
		scrHard->Init(16);
	#endif // CONF_LCD_24
#endif // CONF_LCD

#ifdef CONF_KEY4
	key = (Key *)Key4::init();
#endif // CONF_KEY4


#ifdef CONF_WATHDOG
	ns_watchdog::Init(WDTO_500MS);
	wathDogStart = true;
#endif // CONF_WATHDOG

#ifdef CONF_RS232
	serial = RS232::init(baud9600, DISABLE, bit8, 32, 32);
#endif // CONF_RS232

#ifdef CONF_SPI_SOFT
	spi = (Spi *) new Spi_soft;
#endif // CONF_SPI_SOFT
//
	ns_user::init();

#ifdef CONF_MENU
	ns_menu::init();
#endif // CONF_MENU
}

void	Core::interrupt0()
{
	TaskQueueUnit	*unit;
	for (uint8_t i = 0; i < taskQueueHigh_size; i++)
	{
		unit = &taskQueueHigh[i];
		(unit->cur_tik)++;
		if (unit->cur_tik >= unit->div_tik)
		{
			unit->cur_tik = 0;
			callMetod(unit->metod);
		}
	}
	// --------------Idle--------------------
	for (uint8_t i = 0; i < taskQueueIdle_size; i++)
	{
		unit = &taskQueueIdle[i];
		if (unit->cur_tik < unit->div_tik)	(unit->cur_tik)++;
	}
}

void	Core::interrupt2()
{
	sei();
	if (taskQueueMidle_size != 0)
	{
		TaskQueueUnit	*unit;
		for (uint8_t i = 0; i < taskQueueMidle_size; i++)
		{
			unit = &taskQueueMidle[i];
			(unit->cur_tik)++;
			if (unit->cur_tik >= unit->div_tik)
			{
				unit->cur_tik = 0;
				callMetod(unit->metod);
			}
		}
	}
#ifdef CONF_WATHDOG
// 	if (wathDogStart)	
// 		ns_watchdog::ResetCount();
#endif // CONF_WATHDOG
}

void	Core::idleCycle() {
	if (taskQueueIdle_size != 0)
	{
		if (taskQueueIdle_cur >= taskQueueIdle_size)	taskQueueIdle_cur = 0;
		uint8_t	cur, div;
		CRITICAL_SECTION
		{
			cur = taskQueueIdle[taskQueueIdle_cur].cur_tik;
			div = taskQueueIdle[taskQueueIdle_cur].div_tik;
		}
		if (cur >= div)
		{
			CRITICAL_SECTION
			{
				taskQueueIdle[taskQueueIdle_cur].cur_tik = 0;
			}
			callMetod(taskQueueIdle[taskQueueIdle_cur].metod);
		}
		taskQueueIdle_cur++;
	}
// 	CRITICAL_SECTION
// 	{
// 		spaceCount++;
// 	}
// 	#ifdef CONF_WATHDOG
// 	wathDogStart = false;
// 	ns_watchdog::ResetCount();
// 	#endif // CONF_WATHDOG
}

/*void	Core::spacePrint()
{
	uint16_t d;
	static	uint16_t	us_old = 0, us_n = 0;
	static	uint8_t		us_c = 0;
	CRITICAL_SECTION
	{
		d = spaceCycle;
	}
	us_n += d;
	CRITICAL_SECTION
	{
		us_c++;
	}
	if (us_c >= 4)
	{
		us_c = 0;
		us_old = us_n / 4;
		us_n = 0;
	}
	scr->DigitZ(30, 3, us_old);
}

void	Core::spaceCountFix()
{
	spaceCycle = spaceCount;
	spaceCount = 0;
}
*/
