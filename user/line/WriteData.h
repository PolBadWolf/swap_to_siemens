/* 
* WriteData.h
*
* Created: 28.05.2024 16:10:29
* Author: User
*/


#ifndef __WRITEDATA_H__
#define __WRITEDATA_H__

#include "core/core.h"

class WriteData
{
//
public:
	enum StatWork
	{
		offline		= 0,			// отключено
		modOff		= 1,			// ожидание фазы начала для отмены
		sendEnd		= 2,			// передача завершена штатным образом
		delay		= 3,			// ожидание следущего режима
		// --------
		phaze1		= 11,			// готовность спад
		//phaze1_2	= 12,			// старт фронт
		//
		phaze2_1	= 21,			// sproket спад
		phaze2_2	= 22,			// вывод данных, строб
		phaze2_3	= 23,			// sproket фронт
		//
		phaze3_1	= 31,			// старт спад
		phaze3_2	= 32,			// готовность фронт
		// -------------
		error		= 128,			// не известная ошибка системы
		error_delay	= 129			// ошибка в модуле задержки
	};
//variables
public:
	uint8_t					error_sim;
	uint8_t					sim_starDat;
	uint16_t				sim_adr;
protected:
private:
	static	WriteData		*obj;
	StatWork				statWork;			// текущее состояние модуля передачи
	uint16_t				sendCountByte;		// количество переданных байт
	StatWork				modeAfterWait;		// режим после ожидания
	uint16_t				countTikDelay;
	volatile	uint8_t		eventOn;			// флаг работы события
	// --
	//uint32_t				sendAdr
// 	static	uint16_t		f_timer;
	volatile	uint8_t		fl_reset;			// флаг сброса работы модуля
//functions
public:
	static	WriteData*		init();
	static	void			timerFast_irq();
	void		sendOn();
	void		sendOff();
	uint8_t		getStatusWork();
	uint16_t	getSendCountByte();
protected:
private:
public:
	WriteData();
	~WriteData();
	WriteData( const WriteData &c );
	WriteData& operator=( const WriteData &c );
	void		timerFast();
	void		initPorts();
	void		modeDelay(StatWork mode, uint16_t	tik);
	// -----
	void	mode_delay();
	// -----
	void	mode_phaze1();			// старт
	//
	void	mode_phaze2_1();
	void	mode_phaze2_2();
	void	mode_phaze2_3();
	//
	void	mode_phaze3_1();
	void	mode_phaze3_2();
	

}; //WriteData

#endif //__WRITEDATA_H__
