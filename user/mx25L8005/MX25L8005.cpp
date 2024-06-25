/* 
* MX25L8005.cpp
*
* Created: 26.04.2024 14:26:11
* Author: User
*/

#include <avr\pgmspace.h>

#include "MX25L8005.h"
#include "core/delay.h"
#include "user/mainUser.h"
#include "user/var.h"
// --------
#include "communication/spi/Spi_Hard.h"

// write enable
#define			MX_WREN		0x06
// write disable
#define			MX_WRDI		0x04
// read identification
#define			MX_RDID		0x9f
// read status register
#define			MX_RDSR		0x05
// write status register
#define			MX_WRSR		0x01
// read data
#define			MX_READ		0x03
// fast read data
#define			MX_FastRead	0x0b
// sector erase
#define			MX_SE		0x20
// block erase
#define			MX_BE		0x52
// chip erase
#define			MX_CE		0x60
// page program
#define			MX_PP		0x02
// read electronic ID
#define			MX_RES		0xab
// read electronic manufacturer & Device ID
#define		MX_REMS		0x90


MX25L8005	*MX25L8005::obj = 0;

// default constructor
MX25L8005::MX25L8005()
{
} //MX25L8005

// default destructor
MX25L8005::~MX25L8005()
{
} //~MX25L8005

MX25L8005*	MX25L8005::init()
{
	if (obj == 0)	obj = new MX25L8005;
	obj->to			= 0;
	obj->wr_work	= 0;
	obj->wr_Flsend	= 0;
	// -----------------------
// #ifndef		__DEBUG_SIMULYATION
	__delay_ms(100);
// #endif
	// -----------------------
	return	obj;
}

uint16_t	MX25L8005::getID()
{
	uint16_t	id = 0;
	SPI_SS = 0;
	__builtin_avr_delay_cycles(1);
	ns_user::spi_h->transferByte(MX_REMS);
	ns_user::spi_h->transferByte(0x00);
	ns_user::spi_h->transferByte(0x00);
	ns_user::spi_h->transferByte(0x00);
	word_to_byte(id).High	= ns_user::spi_h->transferByte(0x00);
	word_to_byte(id).Low	= ns_user::spi_h->transferByte(0x00);
	SPI_SS = 1;
	return	id;
}

uint8_t	MX25L8005::getStatus()
{
	uint8_t	stat;
	uint8_t	flag = 0;
	transferNop();
	SPI_SS = 0;
	__builtin_avr_delay_cycles(1);
	ns_user::spi_h->transferByte(MX_RDSR);
	if (ns_user::spi_h->statTransferByte != 0)	flag = 1;
	stat = ns_user::spi_h->transferByte(0);
	if (ns_user::spi_h->statTransferByte != 0)	flag = 1;
	SPI_SS = 1;
	if (flag)	mx_fileds_status(stat).timeOut	= 1;
	else		mx_fileds_status(stat).timeOut	= 0;
	return	stat;
}

void	MX25L8005::waitReady()
{
	uint8_t stat;
	for(;;)
	{
		stat = getStatus();
		if (mx_fileds_status(stat).WIP == 0)	break;
		__delay_ms(1);
	}
}

uint8_t	MX25L8005::waitReady(uint16_t	ms)
{
	uint8_t stat;
	for(;;)
	{
		stat = getStatus();
		if ((mx_fileds_status(stat).WIP == 0) && (mx_fileds_status(stat).timeOut == 0))
		{
			stat = 0;
			break;
		}
		if (ms == 0)
		{
			stat = 5;
			break;
		}
		ms--;
		__delay_ms(1);
	}
	return	stat;
}

void	MX25L8005::transferArray(uint8_t	*mass, uint8_t	length)
{
	uint8_t	i, byte;
	for (i = 0; i < length; i++)
	{
		byte = ns_user::spi_h->transferByte(mass[i]);
		mass[i] = byte;
	}
}

uint8_t	MX25L8005::readArray(uint8_t	*mass, uint16_t	length, uint32_t	adr)
{
	if (waitReady(1500) != 0)	return	5;
	uint8_t	stat = 0;
	SPI_SS = 0;
	do 
	{
		ns_user::spi_h->transferByte(MX_READ);
		if (ns_user::spi_h->statTransferByte != 0)	{ stat = 1; break; }
		
		ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte3);
		if (ns_user::spi_h->statTransferByte != 0)	{ stat = 1; break; }
		
		ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte2);
		if (ns_user::spi_h->statTransferByte != 0)	{ stat = 1; break; }
		
		ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte1);
		if (ns_user::spi_h->statTransferByte != 0)	{ stat = 1; break; }
		
		for (uint16_t i = 0; i < length; i++)
		{
			mass[i] = ns_user::spi_h->transferByte(0);
			if (ns_user::spi_h->statTransferByte != 0)	{ stat = 2; break; }
		}
	} while (false);
	SPI_SS = 1;
	return stat;
}

uint8_t	MX25L8005::writeArray(uint8_t	*mass, uint16_t	length, uint32_t	adr)
{

	uint8_t	stat = getStatus();
	if (mx_fileds_status(stat).SRWD != 0)	return	3;		// проверка защита от записи

	uint8_t		basePage = ~((uint8_t)(adr / 0x100));		// базовый блок не равен текущему для первого пуска
	uint8_t		currPage, flag = 0;							// флаг блока 256 байт в работе
	
	stat = 0;
	for (uint16_t i = 0; i < length; i++, adr++)
	{
		currPage = (uint8_t)(adr / 0x100);
		if (basePage != currPage)
		{
			basePage = currPage;
			if (flag != 0)
			{
				SPI_SS = 1;
				__delay_ms(MX_TIME_WRITE_PAGE);
				flag = 0;
			}
			// ----------
			writeEnable();
			transferNop();
			SPI_SS = 0;
			do 
			{
				flag = 1;
				ns_user::spi_h->transferByte(MX_PP);
				if (ns_user::spi_h->statTransferByte != 0)	{ stat = 4; break; }
				
				ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte3);
				if (ns_user::spi_h->statTransferByte != 0)	{ stat = 4; break; }
				
				ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte2);
				if (ns_user::spi_h->statTransferByte != 0)	{ stat = 4; break; }
				
				ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte1);
				if (ns_user::spi_h->statTransferByte != 0)	{ stat = 4; break; }
				
			} while (false);
			// ----------
		}
		// --------
		ns_user::spi_h->transferByte(mass[i]);
		if (ns_user::spi_h->statTransferByte != 0)	{ stat = 4; break; }
		// --------
	}
	if (flag != 0)
	{
		SPI_SS = 1;
		writeDisable();
// 		__delay_ms(1);
		__delay_ms(MX_TIME_WRITE_PAGE);
		flag = 0;
	}
	// ------------------------------------
// 	do
// 	{
// 		stat = getStatus();
// 	} while (mx_fileds_status(stat).WIP);
	return	stat;
}

void	MX25L8005::writeEnable()
{
	SPI_SS = 0;
	ns_user::spi_h->transferByte(MX_WREN);
	SPI_SS = 1;
}

void	MX25L8005::writeDisable()
{
	SPI_SS = 0;
	ns_user::spi_h->transferByte(MX_WRDI);
	SPI_SS = 1;
}

uint8_t	MX25L8005::eraseSector(uint32_t	adr)
{
	adr &= 0x0ff000;
	waitReady();
// 	waitTimeOut();
	uint8_t	stat = getStatus();
	if (mx_fileds_status(stat).SRWD != 0)	return	1;
	writeEnable();
	SPI_SS = 0;
	ns_user::spi_h->transferByte(MX_SE);
	ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte3);
	ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte2);
	ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte1);
	SPI_SS = 1;
	writeDisable();
	__delay_ms(MX_TIME_ERASE_SECTOR);
	do 
	{
		stat = getStatus();
	} while (mx_fileds_status(stat).WIP);
// 	CRITICAL_SECTION
// 	{
// 		to = 120;
// 	}
	return	0;
}

uint8_t	MX25L8005::eraseBlock (uint32_t	adr)
{
	adr &= 0x0f0000;
	waitReady();
	// 	waitTimeOut();
	uint8_t	stat = getStatus();
	if (mx_fileds_status(stat).SRWD != 0)	return	1;
	writeEnable();
	SPI_SS = 0;
	ns_user::spi_h->transferByte(MX_BE);
	ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte3);
	ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte2);
	ns_user::spi_h->transferByte(dWord_to_byte(adr).Byte1);
	SPI_SS = 1;
	writeDisable();
	__delay_ms(MX_TIME_ERASE_BLOCK);
	do
	{
		stat = getStatus();
	} while (mx_fileds_status(stat).WIP);
	// 	CRITICAL_SECTION
	// 	{
	// 		to = 120;
	// 	}
	return	0;
}

void	MX25L8005::mx_irq()
{
	// передача данных на лету
	obj->wr_mem();
}

void	MX25L8005::transferNop()
{
	for (uint8_t i = 0; i < 8; i++)
	{
		ns_user::spi_h->transferByte(0);
	}
}

void	MX25L8005::fWr_init(uint32_t	adr)
{
	CRITICAL_SECTION
	{
		wr_adr		= adr;
		wr_head		= 0;
		wr_tail		= 0;
		wr_error	= 0;
		wr_Flsend	= 0;
		wr_work		= 1;
		wr_timeCount_pp = 0;
	}
}

void	MX25L8005::fWr_dataSend(uint8_t	dat)
{
	if (wr_work == 0)	return;
	CRITICAL_SECTION
	{
		wr_buff[wr_head] = dat;
		incIndxBuff(&wr_head);
	}
}

void	MX25L8005::wr_mem()
{
	if (wr_work == 0)	return;
	// ------------------
	if (wr_timeCount_pp > 0)
	{
		wr_timeCount_pp--;
		return;
	}
	// ------------------
	if (wr_Flsend == 0)
	{
		uint16_t	head;
		// проверка заполнености буффера
		CRITICAL_SECTION
		{
			head = wr_head;
		}
		// коррекция для сравнения
		if (head < wr_tail)		head += MX_LEN_BUFFER;
		// преодоление порога
		if ((head - wr_tail) < MX_THRESHOLD)	return;
		// включить флаг передачи
		wr_Flsend = 1;
		// команда записи
		writeEnable();
		transferNop();
		SPI_SS = 0;
		do
		{
			ns_user::spi_h->transferByte(MX_PP);
			if (ns_user::spi_h->statTransferByte != 0)	{ wr_error = 1; break; }
		
			ns_user::spi_h->transferByte(dWord_to_byte(wr_adr).Byte3);
			if (ns_user::spi_h->statTransferByte != 0)	{ wr_error = 1; break; }
		
			ns_user::spi_h->transferByte(dWord_to_byte(wr_adr).Byte2);
			if (ns_user::spi_h->statTransferByte != 0)	{ wr_error = 1; break; }
		
			ns_user::spi_h->transferByte(dWord_to_byte(wr_adr).Byte1);
			if (ns_user::spi_h->statTransferByte != 0)	{ wr_error = 1; break; }
		} while (false);
		// --
		uint8_t dat;
		for (;;)
		{
			// проверка буффера на опустошение
			CRITICAL_SECTION
			{
				head = wr_head;
			}
			if (head == wr_tail)	break;
			// отправка байта на запись
			dat = wr_buff[wr_tail];
			ns_user::spi_h->transferByte(dat);
			if (ns_user::spi_h->statTransferByte != 0)
			{	// ошибка отправки
				wr_error = 1;
				break;
			}
			// +1
			uint32_t wr_adr_tmp;
			CRITICAL_SECTION
			{
				incIndxBuff(&wr_tail);
				wr_adr++;
				wr_adr_tmp = wr_adr;
			}
			// проверка на окончание страницы
			if ((wr_adr_tmp & 0xff) == 0)	break;
		}
		SPI_SS = 1;
		writeDisable();
// 		if ((wr_adr == 0) || (wr_error != 0))
// 		{
			CRITICAL_SECTION
			{
				wr_Flsend	= 0;
// 				wr_work		= 0;
			}
// 		}
		wr_timeCount_pp = 5 - 1;
	}
}

void	MX25L8005::fWr_endSend()
{
	do 
	{
		scr->String_P(1, PSTR("wr_fl="));
		scr->DigitZ(7, 3, wr_Flsend);
	} while (wr_Flsend != 0);
	CRITICAL_SECTION
	{
		wr_work	= 0;
	}
	uint8_t	sendPage = 0;
	uint8_t	dat;
	for (;;)
	{
		// проверка буффера на опустошение
		if (wr_head == wr_tail)	break;	// буффер выгружен
		// проверка на начало передачи
		if (sendPage == 0)
		{	// заголовок передачи
			sendPage = 1;
			// разрешить запись
			writeEnable();
			transferNop();
			SPI_SS = 0;
			// команда записи с указанного адреса
			ns_user::spi_h->transferByte(MX_PP);
			ns_user::spi_h->transferByte(dWord_to_byte(wr_adr).Byte3);
			ns_user::spi_h->transferByte(dWord_to_byte(wr_adr).Byte2);
			ns_user::spi_h->transferByte(dWord_to_byte(wr_adr).Byte1);
		}
		// отправка байта из буффера на запись
		dat = wr_buff[wr_tail];
		ns_user::spi_h->transferByte(dat);
		// инкримент указателя буфера и адреса
		uint32_t wr_adr_tmp;
		CRITICAL_SECTION
		{
			incIndxBuff(&wr_tail);
			wr_adr++;
			wr_adr_tmp = wr_adr;
		}
		// проверка на окончание страницы
		if ((wr_adr_tmp & 0xff) == 0)
		{	// завершение передачи страницы
			sendPage = 0;
			SPI_SS = 1;
			// запретить запись
			writeDisable();
			// время записи страницы
			__delay_ms(MX_TIME_WRITE_PAGE);
		}
	}
	// провера на начатую запись страницы
	if (sendPage != 0)
	{	// завершение записи страницы
		sendPage = 0;
		SPI_SS = 1;
		// запретить запись
		writeDisable();
		// время записи страницы
		__delay_ms(MX_TIME_WRITE_PAGE);
	}
}


void	MX25L8005::incIndxBuff(uint16_t *indx)
{
	(*indx)++;
	if (*indx >= MX_LEN_BUFFER)	*indx = 0;
}

uint32_t	MX25L8005::get_wr_adr()
{
	return	wr_adr;
}

// ==================================================
void	MX25L8005::fRd_init(uint32_t adr, uint16_t lenght)
{
	rd_adr		= adr;
	rd_lenght	= lenght;
	rd_point	= 128;
}

uint8_t	MX25L8005::fRd_readByte(uint8_t	*dat)
{
	if (rd_lenght == 0)	return 0;
	// загрузка буффера
	if (rd_point >=128)
	{
		rd_point = 0;
		uint8_t len = 128;
		if (rd_lenght < 128)
		{
			len = rd_lenght;
		}
		readArray(wr_buff, len, rd_adr);
	}
	*dat = wr_buff[rd_point];
	rd_point++;
	rd_adr++;
	uint8_t fl_out;
	CRITICAL_SECTION
	{
		if (rd_lenght > 0)
		{
			rd_lenght--;
		}
		fl_out = rd_lenght > 0;
	}
	return	fl_out;
}

void	MX25L8005::fRd_end()
{
	CRITICAL_SECTION
	{
		rd_lenght = 0;
	}
}

