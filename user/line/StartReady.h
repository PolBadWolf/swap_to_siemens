/* 
* StartReady.h
*
* Created: 17.10.2024 9:40:23
* Author: User
*/


#ifndef __STARTREADY_H__
#define __STARTREADY_H__

#include "core/core.h"

#define STARTREADY_STAT_ON		1
#define STARTREADY_STAT_OFF		0
#define STARTREADY_STAT_WAIT	2

typedef		void(*StartReadyTypeMetod)(uint8_t	stat);
#define		callMetodStartReady(metod, arg)	((StartReadyTypeMetod)(metod))(arg);

class StartReady
{
//variables
public:
protected:
private:
	static		StartReady		*obj;
	uint8_t		stat_st;
	uint8_t		stat_fl;
	StartReadyTypeMetod		externMetod;
	uint8_t		fStart;
//functions
public:
	~StartReady();
	static	StartReady*		init(); //StartReadyTypeMetod mtRead, StartReadyTypeMetod mtSend);
	static	StartReady*		getObj();
	uint8_t	getStatFl();
	void	resetFlag();
	static	void	interrupt();
	void	irqOn(StartReadyTypeMetod externMetod);
	static	void	irqOff();
protected:
private:
	StartReady( const StartReady &c );
	StartReady& operator=( const StartReady &c );
	StartReady();
	void	interrupt_int();

}; //StartReady

#endif //__STARTREADY_H__
