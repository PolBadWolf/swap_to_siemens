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
		Wait_ByteCompletion	= 6,		// �������� ���������� ������ �����
		Wait_ByteRead		= 7			// �������� ������ ��� ������ �����
	};

//variables
public:
protected:
private:
//public:
	static	ReadData	*obj;
	uint8_t			blockRead;			// ���������� ������� ������
	StatWork		statWork;			// ������� ������ ������

//functions
public:
	static	ReadData*	init();
	static	void	timerFast_irq();	// ������ ������ ������
	void	timerFast();				// ������ ������ ������
	void	initPorts();				// ��������� ������ �� �����
	// -----------
	uint8_t	getStatWork();				// ������ ������ ��������� ������
	uint8_t	readOn();					// ��������� ������
	uint8_t readOff();
	void	cancel();					// ������ ������
	void	reset();					// ����� ������ ��� ������
protected:
private:
	ReadData();
	~ReadData();
	ReadData( const ReadData &c );
	ReadData& operator=( const ReadData &c );
	// -------------------------------------
	void	int_Wait_InitialState();	// �������� ��������� ��������� ��� ������ ������
	void	int_Wait_StartRead();		// �������� ������� ����� ������
	void	int_Wait_ByteCompletion();	// �������� ���������� ������ �����
	void	int_Wait_ByteRead();		// �������� ������ ��� ������ �����
}; //ReadData

#endif //__READDATA_H__
