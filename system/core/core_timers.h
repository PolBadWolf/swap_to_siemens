/*
 * core_timers.h
 *
 * Created: 07.06.2024 9:04:01
 *  Author: User
 */ 


#ifndef CORE_TIMERS_H_
#define CORE_TIMERS_H_

#include "timers/Timer0.h"
#include "timers/Timer2.h"

#define	timer0_FEQ	4000
#define	timer2_FEQ	2000

// system obj
namespace	Core
{
	extern	Timer0		*timer0;
	extern	Timer2		*timer2;
}


#endif /* CORE_TIMERS_H_ */