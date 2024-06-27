/* 
* Simulyation.cpp
*
* Created: 26.06.2024 13:17:10
* Author: User
*/


#include "Simulyation.h"

#include "user/line/signal_pin.h"
#include <avr\pgmspace.h>
#include "core\delay.h"
#include "core\core_timers.h"

#define TIMER_X		timer0_FEQ

#define		__read_sprocket_down	(1 * TIMER_X / 1000)
#define		__read_sprocket_up		(2 * TIMER_X / 1000)

#define		__ADR_TO_DATA

// private metods
namespace	ns_simul
{
	void	read_setDelay(uint16_t	tik);
}

// public vars
namespace	ns_simul
{
	const	uint16_t	flsh_avr_startAdr	= 0x40;
	uint16_t	read_avr_adr;
	uint16_t	read_lenght;
	//
	enum StatRead
	{
		OffLine		= 0,
		Init		= 1,
		StartInit	= 2,
		SprkDown	= 3,
		DataOut		= 4,
		EndSend		= 5
	};
}

// private vars
namespace	ns_simul
{
	StatRead		statRead;
	uint16_t		readDelay	= 0;
}

void	ns_simul::read_init()
{
	statRead = OffLine;
}

void	ns_simul::read_On()
{
	statRead = Init;
}

void	ns_simul::read_setDelay(uint16_t	tik)
{
	CRITICAL_SECTION
	{
		readDelay = tik;
	}
}

void	ns_simul::read_cycle()
{
	uint16_t	readDelay_cur;
	if (statRead == OffLine)	return;
	CRITICAL_SECTION
	{
		if (readDelay > 0)	readDelay--;
		readDelay_cur = readDelay;
	}
	switch (statRead)
	{
		case Init:
			ns_pins::transfer_startStop(0);
			ns_pins::transfer_slewInc(1);
			ns_pins::transfer_sprocket(1);
			statRead = StartInit;
		break;
		case StartInit:
			if (readDelay_cur == 0)
			{
				ns_pins::transfer_startStop(1);
				read_setDelay(__read_sprocket_down);
				statRead = SprkDown;
			}
		break;
		case SprkDown:
			if (readDelay_cur == 0)
			{
				if (read_lenght == 0)
				{
// 					ns_pins::transfer_startStop(0);
					statRead = OffLine; // ********************************************
					break;
				}
				ns_pins::transfer_sprocket(0);
				read_setDelay(__read_sprocket_down);
				statRead = DataOut;
			}
		break;
		case DataOut:
			if (readDelay_cur == 0)
			{
				if (read_lenght == 1)
				{
					ns_pins::transfer_startStop(0);
				}
// 				if (read_lenght == 0)
// 				{
// 					ns_pins::transfer_startStop(0);
// 					statRead = EndSend; // ********************************************
// 					break;
// 				} 
// 				else
				{
					uint8_t vByte;
#ifdef				__ADR_TO_DATA
					vByte = word_to_byte(read_avr_adr).Low;
#else
					vByte = pgm_read_byte(read_avr_adr);
#endif
					ns_pins::transfer_data(vByte);
					read_avr_adr++;
					read_lenght--;
					__delay_us(150);
					ns_pins::transfer_sprocket(1);
					read_setDelay(__read_sprocket_up);
					statRead = SprkDown;
				}
			}
		break;
					ns_pins::transfer_sprocket(1);
					statRead = OffLine;
		case EndSend:
		break;
		default:
		break;
	}
}
