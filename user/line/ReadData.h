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
	uint32_t		wr_lenght;
	uint16_t		countSafeDelay;
	uint16_t		countSafeIntegr;
protected:
private:
//public:
	static	ReadData	*obj;
	uint8_t			blockRead;			// блокировка функции приема
	StatWork		statWork;			// текущий статус приема
	uint32_t		wr_freeSize;
	uint8_t			wr_overSize;
	uint8_t			datDelay;

//functions
public:
	static	ReadData*	init();
	static	void	timerFast_irq();		// таймер приема данных
	void		timerFast();				// таймер приема данных
	void		initPorts();				// настройка портов на прием
	void		setStatWork(StatWork statusWork);
	// -----------
	uint8_t		getStatWork();						// статус работы программы приема
	uint8_t		readOn(uint32_t freeSize);			// включение приема
	uint8_t		readOff();
	void		cancel();							// отмена приема
	void		reset();							// сброс ошибки или отмены
	uint8_t		checkErrorParity(uint8_t dat);		// проверка четности
	uint16_t	getWrFreeSize();
	uint8_t		getWrOverSize();
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
	void	serialDataSend(uint8_t dat);
	// --------
	void	transfer_readByte();
}; //ReadData

#endif //__READDATA_H__
