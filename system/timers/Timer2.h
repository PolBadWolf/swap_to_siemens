/* 
* Timer2.h
*
* Created: 10.04.2024 14:33:51
* Author: User
*/


#ifndef __TIMER2_H__
#define __TIMER2_H__

#include "core/core_def.h"

class Timer2
{
//variables
public:
	static	uint16_t		F_TIMER;
protected:
private:

//functions
public:
	Timer2(uint16_t fclk);
	~Timer2();
protected:
private:
	Timer2( const Timer2 &c );
	Timer2& operator=( const Timer2 &c );

}; //Timer2

#endif //__TIMER2_H__
