/* 
* MX25L8005.h
*
* Created: 26.04.2024 14:26:11
* Author: User
*/


#ifndef __MX25L8005_H__
#define __MX25L8005_H__

#include "core/core.h"
#define		MX_LEN_BUFFER		512
#define		MX_THRESHOLD		0

#define		MX_TIME_WRITE_PAGE			5
#define		MX_TIME_ERASE_SECTOR		120
#define		MX_TIME_ERASE_BLOCK			2000

typedef struct
{
	unsigned	WIP:1;
	unsigned	WEL:1;
	unsigned	BP0:1;
	unsigned	BP1:1;
	unsigned	BP2:1;
	unsigned	timeOut:1;
	unsigned	none:1;
	unsigned	SRWD:1;
} MX_STATUS;

#define		mx_fileds_status(x)		(*((volatile MX_STATUS*)(&x)))

class MX25L8005
{
//variables
public:
	uint8_t		wr_buff[MX_LEN_BUFFER];
	const	static	uint16_t	idDev = 0xc213;
protected:
private:
public:
	static	MX25L8005	*obj;
	uint16_t	to;
	uint32_t	wr_adr;					// текущий адрес при по
	uint16_t	wr_head;
	uint16_t	wr_tail;
	uint8_t		wr_work;				// флаг разрешение/работы передачи "на лету"
	uint8_t		wr_Flsend;				// флаг выполния передачи субсектора "на лету"
	uint8_t		wr_error;
	// ------------
	uint32_t	rd_adr;					// адрес чтения
	uint16_t	rd_lenght;				// длина программы
	uint8_t		rd_point;				// точка чтения с буффера
	// ------------
	uint8_t		wr_timeCount_pp;			// счетчик времени записи по функции PP
//functions
public:
	static	MX25L8005*	init();
	static	void		mx_irq();
	// -------------------
	uint16_t	getID();
	uint8_t		getStatus();
	void		waitReady();
	uint8_t		waitReady(uint16_t	ms);
	uint8_t		readArray(uint8_t	*mass, uint16_t	length, uint32_t	adr);
	uint8_t		writeArray(uint8_t	*mass, uint16_t	length, uint32_t	adr);
	uint8_t		eraseSector(uint32_t	adr);
	uint8_t		eraseBlock (uint32_t	adr);
	// --
	void		fWr_init(uint32_t	adr);
	void		fWr_dataSend(uint8_t	dat);
	void		fWr_endSend();
	uint32_t	get_wr_adr();
	// --
	void		fRd_init(uint32_t adr, uint16_t lenght);
	uint8_t		fRd_readByte(uint8_t	*dat);
	void		fRd_end();
protected:
private:
	MX25L8005();
	~MX25L8005();
	MX25L8005( const MX25L8005 &c );
	MX25L8005& operator=( const MX25L8005 &c );
	//
	void		transferArray(uint8_t	*mass, uint8_t	lenght);
	void		writeEnable();
	void		writeDisable();
// 	void		waitTimeOut();
	void		transferNop();
	//
	void		wr_mem();
	inline		void		incIndxBuff(uint16_t *indx);
}; //MX25L8005

#endif //__MX25L8005_H__
