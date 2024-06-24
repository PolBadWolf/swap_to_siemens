/*
 * delay.cpp
 *
 * Created: 08.04.2024 15:59:48
 *  Author: User
 */ 

#pragma GCC optimize("O0")

#include "delay.h"
#include "core/core_def.h"

static volatile	uint16_t	delay_tik = 0;
static volatile	uint16_t	tik_freeze = 0;

void	delay_irq() {
	delay_tik++;
}

void	__delay_ms_irq(uint16_t	_ms) {
	
	volatile bool	fl_exit = false;
	CRITICAL_SECTION
	{
		delay_tik = 0;
	}
	do 
	{
		CRITICAL_SECTION
		{
			if (delay_tik >= _ms)	{
				fl_exit = true;
				tik_freeze = delay_tik;
			}
		}
		if (fl_exit)	break;
	} while (1);
}
