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
		offline		= 0,			// ���������
		modOff		= 1,			// �������� ���� ������ ��� ������
		sendEnd		= 2,			// �������� ��������� ������� �������
		delay		= 3,			// �������� ��������� ������
		// --------
		phaze1		= 11,			// ���������� ����
		//phaze1_2	= 12,			// ����� �����
		//
		phaze2_1	= 21,			// sproket ����
		phaze2_2	= 22,			// ����� ������, �����
		phaze2_3	= 23,			// sproket �����
		//
		phaze3_1	= 31,			// ����� ����
		phaze3_2	= 32,			// ���������� �����
		// -------------
		error		= 128,			// �� ��������� ������ �������
		error_delay	= 129			// ������ � ������ ��������
	};
//variables
public:
	uint8_t					error_sim;
	uint8_t					sim_starDat;
	uint16_t				sim_adr;
protected:
private:
	static	WriteData		*obj;
	StatWork				statWork;			// ������� ��������� ������ ��������
	uint16_t				sendCountByte;		// ���������� ���������� ����
	StatWork				modeAfterWait;		// ����� ����� ��������
	uint16_t				countTikDelay;
	volatile	uint8_t		eventOn;			// ���� ������ �������
	// --
	//uint32_t				sendAdr
// 	static	uint16_t		f_timer;
	volatile	uint8_t		fl_reset;			// ���� ������ ������ ������
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
	void	mode_phaze1();			// �����
	//
	void	mode_phaze2_1();
	void	mode_phaze2_2();
	void	mode_phaze2_3();
	//
	void	mode_phaze3_1();
	void	mode_phaze3_2();
	

}; //WriteData

#endif //__WRITEDATA_H__
