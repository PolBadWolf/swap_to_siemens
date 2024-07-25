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
		Offline				= 2,		// ����� ��������
		EndRead				= 3,		// ����� ������
		Wait_InitialState	= 4,		// �������� ��������� ��������� ��� ������ ������
		Wait_StartRead		= 5,		// �������� ������� ����� ������
		Wait_ReadCompletion	= 6,		// �������� ���������� ������ �����
		Wait_ByteRead		= 7			// �������� ������ ��� ������ �����
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
	uint8_t			blockRead;			// ���������� ������� ������
	StatWork		statWork;			// ������� ������ ������
	uint32_t		wr_freeSize;
	uint8_t			wr_overSize;
	uint8_t			datDelay;

//functions
public:
	static	ReadData*	init();
	static	void	timerFast_irq();		// ������ ������ ������
	void		timerFast();				// ������ ������ ������
	void		initPorts();				// ��������� ������ �� �����
	void		setStatWork(StatWork statusWork);
	static void int_readByte();
	// -----------
	uint8_t		getStatWork();						// ������ ������ ��������� ������
	uint8_t		readOn(uint32_t freeSize);			// ��������� ������
	void		readOff();
	void		cancel();							// ������ ������
	void		reset();							// ����� ������ ��� ������
	uint8_t		checkErrorParity(uint8_t dat);		// �������� ��������
	uint16_t	getWrFreeSize();
	uint8_t		getWrOverSize();
protected:
private:
	ReadData();
	~ReadData();
	ReadData( const ReadData &c );
	ReadData& operator=( const ReadData &c );
	// -------------------------------------
	void	int_Wait_InitialState();	// �������� ��������� ��������� ��� ������ ������
	void	int_Wait_StartRead();		// �������� ������� ����� ������
	void	int_Wait_ReadCompletion();	// �������� ���������� ������ �����
	void	int_Wait_ByteRead();		// �������� ������ ��� ������ �����
	void	serialDataSend(uint8_t dat);
	// --------
	void	transfer_readByte();
}; //ReadData

#endif //__READDATA_H__
