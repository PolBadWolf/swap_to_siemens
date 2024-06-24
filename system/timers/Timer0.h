/* 
* Timer0.h
*
* Created: 04.04.2024 8:07:18
* Author: User
*/


#ifndef __TIMER0_H__
#define __TIMER0_H__

#include "core/core_def.h"


class Timer0
{
//variables
public:
	static	uint16_t	F_TIMER;
protected:
private:

//functions
public:
	Timer0(uint16_t	fclk);
	~Timer0();
protected:
private:
	Timer0( const Timer0 &c );
	Timer0& operator=( const Timer0 &c );

}; //Timer0

#endif //__TIMER0_H__
