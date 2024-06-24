/* 
* ReadData.h
*
* Created: 16.05.2024 15:06:41
* Author: User
*/


#ifndef __READDATA_H__
#define __READDATA_H__

#include "core/core.h"

class ReadData
{
public:
//
	enum StatWork
	{
		Cancel				= 0,
		Error				= 1,
		Offline				= 2,		// прием отключен
		EndRead				= 3,		// конец приема
		Wait_InitialState	= 4,		// ожидание исходного состояния для начала приема
		Wait_StartRead		= 5,		// ожидание сигнала старт приема
		Wait_ByteCompletion	= 6,		// ожидание завершение приема байта
		Wait_ByteRead		= 7			// ожидание фронта для приема байта
	};

//variables
public:
protected:
private:
//public:
	static	ReadData	*obj;
	uint8_t			blockRead;			// блокировка функции приема
	StatWork		statWork;			// текущий статус приема

//functions
public:
	static	ReadData*	init();
	static	void	timerFast_irq();	// таймер приема данных
	void	timerFast();				// таймер приема данных
	void	initPorts();				// настройка портов на прием
	// -----------
	uint8_t	getStatWork();				// статус работы программы приема
	uint8_t	readOn();					// включение приема
	uint8_t readOff();
	void	cancel();					// отмена приема
	void	reset();					// сброс ошибки или отмены
protected:
private:
	ReadData();
	~ReadData();
	ReadData( const ReadData &c );
	ReadData& operator=( const ReadData &c );
	// -------------------------------------
	void	int_Wait_InitialState();	// ожидание исходного состояния для начала приема
	void	int_Wait_StartRead();		// ожидание сигнала старт приема
	void	int_Wait_ByteCompletion();	// ожидание завершение приема байта
	void	int_Wait_ByteRead();		// ожидание фронта для приема байта
}; //ReadData

#endif //__READDATA_H__
