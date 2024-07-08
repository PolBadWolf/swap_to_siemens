/* 
* uMenu.cpp
*
* Created: 23.04.2024 10:26:20
* Author: User
*/
#pragma GCC optimize("O0")

#include "uMenu.h"

#ifdef CONF_MENU

#include <avr\pgmspace.h>
#include <avr\eeprom.h>
#include "core\core.h"
#include "core\bits.h"
#include "core\delay.h"
#include "user\mainUser.h"
#include "user\system\menuUser.h"
#include "system\menu\menu.h"
#include "user\var.h"
#include "user\line\Simulyation.h"

#include "user/line/signal_pin.h"
#include "system/indication/Lcd_hard.h"
#include <avr/wdt.h>

uint16_t	startCount;

#define		LENGHT_RUN_STRING	0x10

// #define __MENU_DEBUG__

/*	const volatile uint8_t ms[] PROGMEM =
	{
		0xa5, 0x8d, 0x0a,
		0x28, 0xd2, 0x78, 0xbd, 0x35, 0xb7, 0x35, 0xa0,
		0x42, 0x41, 0xd4, 0xd4, 0xd2, 0xc5, 0x53, 0x53, 0x2d, 0x33, 0xb2, 0xb4, 0xaf, 0xb1, 0x30, 0xa0,
		0xeb, 0x2e, 0xb1, 0x2d, 0x35, 0x30, 0xa0,
		0xb2, 0xb7, 0xaf, 0xb1, 0xb2, 0xaf, 0xb2, 0x30, 0xb1, 0xb2, 0xa9, 0x8d, 0x0a,
		0x4e, 0xb1, 0x30, 0x30, 0x30, 0xa0, 0x47, 0x39, 0x30, 0x8d, 0x0a,
		0x4e, 0xb1, 0x30, 0xb1, 0x30, 0xa0, 0x47, 0x30, 0x30, 0xa0, 0xd8, 0x30, 0xa0, 0x5a, 0x30, 0xa0, 0x4d, 0x30, 0x33, 0x8d, 0x0a,
		0x4e, 0xb1, 0x30, 0xb2, 0x30, 0xa0, 0x47, 0x39, 0xb1, 0x8d, 0x0a,
		0x8d, 0x0a
	};*/
// 	const volatile uint8_t ms[1057] PROGMEM = {
	const volatile uint8_t ms[] PROGMEM = {
		"%\r\n"
		"Rx=575 BATTRESS-324/10 k.1-50 27/12/2012)\r\n"
		"N1000 G90\r\n"
		"N1010 G00 X0 Z0 M03\r\n"
		"N1020 G91\r\n"
		"N1030 G95 M35-r\n"
		"N1040 G04 X2000 S11 T101 M08\r\n"
		"N1050 G00 X-131315 Z-171255\r\n"
		"N1052 G01 X-5000 F1200\r\n"
		"N1054 X-1312 Z4100 F1000\r\n"
		"N1056 G00 X5192\r\n"
		"N1058 Z-131000\r\n"
		"N1060 G01 X-2000 F1000\r\n"
		"N1070 X-4162 Z130000 F1200\r\n"
		"N1090 G00 X722\r\n"
		"N1100 Z-8964\r\n"
		"N1110 G01 X-4147 Z8964 F500\r\n"
		"N1120 G00 X35100\r\n"
		"N1130 Z90500\r\n"
		"N1140 G04 X2000 S12 T202\r\n"
		"N1150 G00 X-60774\r\n"
		"N1160 Z-47509\r\n"
		"N1170 G01 X-8371 Z-23000 F1000\r\n"
		"N1180 G00 Z4407\r\n"
		"N1190 G01 X589 F1000\r\n"
		"N1200 X8000 Z13857\r\n"
		"N1210 X15928\r\n"
		"N1220 G00 X53889 Z29000\r\n"
		"N1230 G04 X2000 S08 T505\r\n"
		"N1240 G00 X-42905 Z-54000 M29\r\n"
		"N1250 G33 X4881 Z-152500 I163 K5080\r\n"
		"N1260 G00 X2000\r\n"
		"N1270 Z152500\r\n"
		"N1280 X-7451\r\n"
		"N1290 G33 X4881 Z-152500 I163 K5080\r\n"
		"N1300 G00 X2000\r\n"
		"N1310 Z152500\r\n"
		"N1320 X-7361\r\n"
		"N1330 G33 X4881 Z-152500 I163 K5080\r\n"
		"N1340 G00 X2000\r\n"
		"N1350 Z152500\r\n"
		"N1360 X-7031\r\n"
		"N1370 G33 X4881 Z-152500 I163 K5080\r\n"
		"N1380 G00 X2000\r\n"
		"N1390 G00 X87876 Z172500 S00 M09\r\n"
		"N1470 G04 X1000 T000\r\n"
		"N1480 G00 X47009 Z98000\r\n"
		"N1485 G04 X2000 T101\r\n"
		"N1490 M30\r\n"
		"\r\n"
	};


void menuUser_1Milisec()
{
	CRITICAL_SECTION
	{
		if (startCount > 0)		startCount--;
	}
}

void	read_string(uint32_t adr)
{
	uint8_t e, sim;
	uint8_t indx = 0;
	uint8_t fl = 0;
	ns_user::flash->serialRead_begin(adr);
	for (e = 0; e <63; e++, indx++)
	{
		sim = ns_user::flash->serialReadByte();
		sim &= 0x7f;
		if ((sim == 0) || (sim == 0x0d) || (sim == 0x10))
		{
			if (fl == 2)
			{
				ns_var::buf_string[indx] = 0;
				break;
			}
		} 
		else
		{
			ns_var::buf_string[indx] = sim;
			fl = 2;
		}
	}
	ns_user::flash->serialRead_end();
}

void	start_init()
{
	key->autoRepeatOn();
	scr->Clear();
	//
	startCount = 100;
}

void	start_count()
{
	uint16_t	tmp;
	CRITICAL_SECTION
	{
		tmp = startCount;
	}
	if (tmp == 0)	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

void	start_setCount()
{
	CRITICAL_SECTION
	{
		startCount	= 2000;
	}
}

void	start_view()
{
	scr->Clear();
	if (ns_var::simulOn)
	{
		scr->String_P(PSTR("режим симуляции"));
		scr->SetPosition2(0, 1);
		scr->String_P( PSTR("размер= ") );
		scr->Digit(5, ns_var::simulLenght);
	} 
	else
	{
		scr->String_P(PSTR("рабочий режим"));
	}
}

void	start_key1()
{
	start_setCount();
	ns_var::simulOn		= 0;
	start_view();
}

void	start_key2()
{
	start_setCount();
	ns_var::simulOn		= 1;
	ns_var::simulLenght	= 200;
	start_view();
}

void	start_key3()
{
	start_setCount();
	ns_var::simulOn		= 1;
	ns_var::simulLenght	= 2000;
	start_view();
}

void	start_key4()
{
	start_setCount();
	ns_var::simulOn		= 1;
	ns_var::simulLenght	= 37200;
	start_view();
}

// -----------------------------------------------------------------
uint32_t	adrRender(uint8_t	nProg)
{
	uint32_t	adr;
	if (ns_var::n_prog < ns_var::blockBoundary)
	{
		adr = nProg * 0x10000;
	}
	else
	{
		adr	= ((uint32_t)(nProg - ns_var::blockBoundary) * 0x1000) + (uint32_t)(ns_var::blockBoundary * 0x10000);
	}
	return	adr;
}

// **************** старт меню *********************
void	user_menu_init()
{
	ns_var::simulOn		= 0;
	//
	ns_var::n_prog		= 10; //0;
	ns_var::adrProg		= adrRender(ns_var::n_prog);
	ns_var::mxMod		= LIST_MOD_read;
	//
	ns_menu::functMenu_aft(_M_START, MENU_SETMODE);
}
// =================================================================
const	uint8_t		progMax = 255 - (ns_var::blockBoundary * 15);
// -----------------------------------------------------------------
void	screen1_vmod()
{
	scr->SetPosition2(9, 0);
	scr->String_P(PSTR("M:"));
	scr->String_P(ns_var::listMod[ns_var::mxMod].name);
}

void	string_findBeginWord()
{
	uint8_t adr = ns_var::buf_string_adr;
	uint8_t sim;
	for (;;)
	{
		sim = ns_var::buf_string[adr];
		if ((sim == 0) || (adr >= 63))
		{
			adr = 0;
			continue;
		}
		if (sim <= ' ')
		{
			adr++;
			continue;
		}
		ns_var::buf_string_adr = adr;
		break;
	}
}


void	screen1_vmem(uint32_t adr_f)
{
	if ((ns_var::n_prog < ns_var::blockBoundary) || (ns_var::buf_string[0] == 0) )
	{
		uint8_t	sb[5];
		ns_user::flash->readArray(sb, 5, adr_f);
		scr->SetPosition2(0, 1);
		for (uint8_t i = 0; i < 5; i++)
		{
			scr->PutChar(' ');
			scr->Hex(sb[i]);
		}
	}
	else
	{
		uint16_t count;
		CRITICAL_SECTION
		{
			count = startCount;
		}
		if (count == 0)
		{
			CRITICAL_SECTION
			{
				startCount = 400;
			}
			uint8_t i, sim;
			uint8_t fl_end = 0;
			uint8_t adr = ns_var::buf_string_adr;
			scr->SetPosition2(0, 1);
			for (i = 0; i < LENGHT_RUN_STRING; i++)
			{
				if (adr >= 64)		adr = 0;
				sim = ns_var::buf_string[adr];
				if ((sim == 0) || (sim == 0x0d) || (sim == 0x0a))
				{
					scr->PutChar(' ');
					fl_end = 1;
					continue;
				}
				if (sim >= ' ')
				{
					scr->PutChar(sim);
					adr++;
				}
				else
				{
					scr->PutChar(' ');
				}
			}
			if (fl_end != 0)
			{
				ns_var::buf_string_adr  = 0;
				CRITICAL_SECTION
				{
					startCount = 2000;
				}
			}
			else				ns_var::buf_string_adr += 1;
		}
	}
	
	/*
	scr->SetPosition2(0, 1);
	for (uint8_t i = 0; i < 3; i++)
	{
		scr->PutChar(' ');
		scr->Hex(ns_var::buf_string[i]);
	}
	scr->Digit(3, ns_var::s_prog);
	*/
}

uint16_t	count_check = 0;

void	screen1_check()
{
	if (ns_var::fl_viewHex != 0)	screen1_vmem(ns_var::adrProg);
	count_check++;
	if (count_check < 100)		return;
	count_check = 0;
	//
	uint16_t id = ns_user::flash->getID();
	#ifdef	__MENU_DEBUG__
	id = 0xc213;
	#endif
	if (id != ns_user::flash->idDev)
	{
		scr->Clear();
		scr->String_P(PSTR("отсутствует"));
		scr->SetPosition2(0, 1);
		scr->String_P(PSTR("накопитель"));
		CRITICAL_SECTION
		{
			uint16_t cx = 0;
			for (;;)
			{
				Lcd_hard::Interrupt_static();
				__delay_ms(1);
				cx++;
				if (cx < 3000)	continue;
				cx = 0;
// 				if (ns_user::flash->getID() == ns_user::flash->idDev)
// 				{
// 					ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
// 					break;
// 				}
				break;
			}
		}
		wdt_enable(1);
		sei();
		for(;;)	__delay_ms(1);
	}
	else
	{
		screen1_view();
	}
}

void	screen1_view()
{
	{
		scr->SetPosition2(4, 0);
		// номер программы
		scr->DigitZ(3, ns_var::n_prog);
		// граница блок и сегмент
		if (ns_var::n_prog < ns_var::blockBoundary)
		{
			scr->PutChar('B');
			ns_var::s_prog = 1;
		}
		else
		{
			scr->PutChar('S');
			ns_var::s_prog = 0;
		}
		// адрес программы
		// 	scr->Hex(dWord_to_byte(ns_var::adrProg).Byte3);
		// 	scr->Hex(dWord_to_byte(ns_var::adrProg).Byte2);
		// 	scr->Hex(dWord_to_byte(ns_var::adrProg).Byte1);
		// 5 байт данных
		screen1_vmem(ns_var::adrProg);
		// режим
		screen1_vmod();
	}
}

int32_t	screen1_findName(uint32_t adr)
{
	uint8_t e;
	uint8_t sim;
	do 
	{
		ns_user::flash->serialRead_begin(adr);
		for (e = 0; ; e++, adr++)
		{
			if (e >= 64) break;
			sim = ns_user::flash->serialReadByte() & 0x7f;
			if (sim != '%')	continue;
			adr++;	e++;
			sim = ns_user::flash->serialReadByte() & 0x7f;
			if (sim != 0x0d)	continue;
			adr++;	e++;
			sim = ns_user::flash->serialReadByte() & 0x7f;
			if (sim != 0x0a)	continue;
			adr++;
			break;
		}
	} while (false);
	ns_user::flash->serialRead_end();
	if (e >= 64)	return -1;
	return	adr;
}

void	screen1_name(uint32_t adr)
{
	if (ns_var::n_prog < ns_var::blockBoundary)
	{
		ns_var::buf_string[0] = 0;
		return;
	}
	ns_var::buf_string_adr = 0;
	ns_var::buf_string_lenght = 0;
	int32_t	adr_name;
	adr_name = screen1_findName(adr);
	if (adr_name < 0)
	{
		ns_var::buf_string[0] = 0;
		return;
	}
	read_string(adr_name);
}

void	screen1_init()
{
	// настройка портов на чтение
	ns_user::readData->initPorts();
	// -------------------------------------
	ns_var::mxMod = 0;
	scr->Clear();
	key->autoRepeatOn();
	scr->String_P(PSTR("Nпр:"));
	ns_var::adrProg = adrRender(ns_var::n_prog);
	screen1_name(ns_var::adrProg + OFFSET_WRITE);
	screen1_view();
}

void	screen1_k2()
{
	if (ns_var::n_prog > 0)
	{
		ns_var::n_prog--;
		ns_var::mxMod = 0;
	}
	ns_var::adrProg = adrRender(ns_var::n_prog);
	screen1_name(ns_var::adrProg + OFFSET_WRITE);
	screen1_view();
}

void	screen1_k3()
{
	if (ns_var::n_prog < progMax)
	{
		ns_var::n_prog++;
		ns_var::mxMod = 0;
	}
	ns_var::adrProg = adrRender(ns_var::n_prog);
	screen1_name(ns_var::adrProg + OFFSET_WRITE);
	screen1_view();
}

void	screen1_k1()
{
	ns_var::mxMod++;
	if (ns_var::mxMod >= LIST_MOD__MAX)
	{
		ns_var::mxMod = 0;
	}
	screen1_vmod();
}

void	screen1_k4()
{
	switch (ns_var::mxMod)
	{
		case LIST_MOD_view:		ns_menu::functMenu_aft(_M_VIEW_BLOCK, MENU_SETMODE);
		break;
		case LIST_MOD_send:		reqeWrite_begin();
		break;
		case LIST_MOD_clr:		ns_menu::functMenu_aft(_M_CLEAR, MENU_SETMODE);
		break;
		case LIST_MOD_copy:		ns_menu::functMenu_aft(_M_COPY, MENU_SETMODE);
		break;
		case LIST_MOD_read:		reqeRead_begin();
		break;
		case LIST_MOD_plus5:	ns_menu::functMenu_aft(_M_PLUS5, MENU_SETMODE);
		break;
		case LIST_MOD_eot:		ns_menu::functMenu_aft(_M_EOT, MENU_SETMODE);
		break;
		case LIST_MOD_pins:		ns_menu::functMenu_aft(_M_PINS, MENU_SETMODE);
		break;
		default:
		break;
	}

}

// ============ copy ========================
void	copy_view()
{
	scr->pos = ns_var::c_pos;
	scr->Digit(3, ns_var::c_prog);
	scr->PutChar(' ');
	uint8_t	b[3];
	ns_user::flash->readArray(b, 1, adrRender(ns_var::c_prog));
	if ((b[0] != 0xff) || (ns_var::n_prog == ns_var::c_prog))
	{
		ns_var::statTarSector = 1;
		scr->PutChar('X');
	}
	else
	{
		ns_var::statTarSector = 0;
		scr->PutChar('O');
	}
	
}

void	copy_init()
{
	key->autoRepeatOn();
	ns_var::c_prog = ns_var::n_prog;
	scr->Clear();
	if (ns_var::n_prog < ns_var::blockBoundary)
	{
		scr->String_P(PSTR("Block "));
		ns_var::s_prog = 1;
	}
	else
	{
		scr->String_P(PSTR("Sector "));
		ns_var::s_prog = 0;
	}
	scr->Digit(3, ns_var::n_prog);
	scr->String_P(PSTR("->"));
// 	scr->String_P(PSTR(" -> "));
	ns_var::c_pos = scr->pos;
	copy_view();
}

void	copy_k2()
{
	if (ns_var::s_prog != 0)
	{	// блочный режим
		if (ns_var::c_prog > 0)		ns_var::c_prog--;
	}
	else
	{	// секторный режим
		if (ns_var::c_prog > ns_var::blockBoundary)		ns_var::c_prog--;
	}
	copy_view();
}

void	copy_k3()
{
	if (ns_var::s_prog != 0)
	{	// блочный режим
		if (ns_var::c_prog < (ns_var::blockBoundary - 1))		ns_var::c_prog++;
	}
	else
	{	// секторный режим
		if (ns_var::c_prog < progMax)		ns_var::c_prog++;
	}
	copy_view();
}

void	copy_k1()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

void	copy_k4()
{
	if (ns_var::statTarSector != 0)		return;
	scr->SetPosition2(0, 1);
	scr->String_P(PSTR("copy: "));
	uint8_t pos = scr->pos;
	uint32_t adrSource = adrRender(ns_var::n_prog);
	uint32_t adrTarget = adrRender(ns_var::c_prog);
	uint16_t n;
	if (ns_var::s_prog != 0)
	{	// режим блок
		n = 65536 / 256;
	}
	else
	{	// режим сектор
		n = 4096 / 256;
	}
	//
// 	uint8_t stat;
	for (uint16_t i = 0; i < n; i++)
	{
		scr->pos = pos;
		scr->Digit(3, 100 * i / 256);
		scr->PutChar('%');
		ns_user::flash->readArray (ns_user::flash->wr_buff, 256, adrSource);
		ns_user::flash->writeArray(ns_user::flash->wr_buff, 256, adrTarget);
		adrSource += 256;
		adrTarget += 256;
	}
	//
	ns_var::n_prog = ns_var::c_prog;
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

// ==================================================

void	clear_init()
{
	key->autoRepeatOff();
	scr->Clear();
	scr->String_P(PSTR("Стирание "));
	scr->Digit(3, ns_var::n_prog);
	scr->SetPosition2(0, 1);
	if (ns_var::n_prog < ns_var::blockBoundary)
	{
		ns_var::s_prog = 1;
		scr->String_P(PSTR("блока ?"));
	}
	else
	{
		ns_var::s_prog = 0;
		scr->String_P(PSTR("сектора ?"));
	}
}

void	clear_no()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

void	clear_yes()
{
	scr->String_P(scr->SetPosition(2, 1), PSTR("clear"));
	//
	uint32_t	addr = adrRender(ns_var::n_prog);
	if (ns_var::s_prog != 0)
	{	// блок
		ns_user::flash->eraseBlock(addr);
	}
	else
	{	// сектор
		ns_user::flash->eraseSector(addr);
	}
	uint16_t n = 0;
	uint8_t pos = scr->SetPosition(6, 1);
	do 
	{
		scr->Digit(pos, 5, n);
		__delay_ms(1);
		n++;
		ns_user::flash->readArray(ns_user::flash->wr_buff, 2, addr);
	} while (ns_user::flash->wr_buff[0] != 0xff);
	//
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}
// =======================================================================
void	reqeRead_init()
{
	// настройка портов на чтение
	ns_user::readData->initPorts();
	// -------------------------------------
	// первая часть читается без запроса
	if (
		(ns_var::ml_adr_n == 0)
	&&	(ns_var::read_fistEnter != 0)
		)
	{
		ns_var::read_fistEnter = 0;
		ns_menu::functMenu_pre(_M_READ_PART, MENU_SETMODE);
		return;
	}
	// -------------------------------------
	scr->Clear();
	scr->String_P(PSTR("часть:"));
	scr->Digit(2, ns_var::ml_adr_n + 1);
	scr->String_P(PSTR("адр:"));
	scr->Hex(word_to_byte(ns_var::ml_adr_offset).High);
	scr->Hex(word_to_byte(ns_var::ml_adr_offset).Low);
	//
	scr->SetPosition2(0, 1);
	scr->String_P(PSTR("чтение? "));
	//
	if (ns_var::ml_adr_n > 0)
	{
		scr->DigitZ(3, ns_var::ml_adr_bd[ns_var::ml_adr_n - 1].offSet_adr);
		scr->PutChar('/');
		scr->DigitZ(3, ns_var::ml_adr_bd[ns_var::ml_adr_n - 1].lenght_adr);
	}
}

void	reqeRead_save()
{
	uint16_t curAdr, curLenght, lenghtBD;
	// если принята хоть одна часть, то сохранить.
	if (ns_var::ml_adr_n > 0)
	{
		ns_user::flash->wr_buff[0] = ns_var::ml_adr_n;
		ns_user::flash->wr_buff[1] = 0xff;
		ns_user::flash->wr_buff[2] = 0xff;
		ns_user::flash->wr_buff[3] = 0xff;
		uint8_t	bi;
		for (uint8_t i = 0; i < ns_var::ml_adr_n; i++)
		{
			curAdr = ns_var::ml_adr_bd[i].offSet_adr;
			curLenght = ns_var::ml_adr_bd[i].lenght_adr;
			bi = (i + 1) * 4;
			ns_user::flash->wr_buff[bi + 0] = word_to_byte(curAdr).Low;
			ns_user::flash->wr_buff[bi + 1] = word_to_byte(curAdr).High;
			ns_user::flash->wr_buff[bi + 2] = word_to_byte(curLenght).Low;
			ns_user::flash->wr_buff[bi + 3] = word_to_byte(curLenght).High;
		}
		lenghtBD = (ns_var::ml_adr_n + 1) * 4;
		ns_user::flash->writeArray(ns_user::flash->wr_buff, lenghtBD, ns_var::ml_adr_base);
	}
	//
	if (ns_var::s_prog != 0)	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
	else
	{
		scr->Clear();
		scr->String_P( PSTR("Принято ") );
		scr->Digit(4, curLenght);
		scr->String_P( PSTR("байт") );
		if (ns_var::error_parity != 0)
		{
			scr->String_P(scr->SetPosition(0, 1), PSTR("ошибка четности") );
		}
		ns_menu::functMenu_aft(_M_WT_SCR1, MENU_SETMODE);
	}
}

void	reqeRead_k2()
{
	reqeRead_save();
}

void	reqeRead_k3()
{	// включение чтения части программы
	ns_menu::functMenu_aft(_M_READ_PART, MENU_SETMODE);
}

void	reqeRead_begin()
{
	if (ns_var::simulOn)
	{
		ns_simul::read_avr_adr		= ns_simul::flsh_avr_startAdr;
		ns_simul::read_lenght		= ns_var::simulLenght;
	}
	//
	ns_var::ml_adr_n = 0;								// номер текущей части программы 0
	ns_var::ml_adr_base = adrRender(ns_var::n_prog);	// базовый адрес программы
	ns_var::read_fistEnter = 1;							// первый запуск
	ns_var::ml_adr_offset = 0;							// оффсет для приема очередной части программы
	ns_user::flash->readArray(ns_user::flash->wr_buff, 2, ns_var::ml_adr_base);
	if (ns_user::flash->wr_buff[0] != 0xff)
	{
		scr->Clear();
		scr->String_P(PSTR("блок/сектор не стерт"));
		__delay_ms(3000);
		ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
		return;
	}
// 	// проверка на сектор/блок
// 	if (ns_var::s_prog == 0)
// 	{
// 		ns_menu::functMenu_pre(_M_READ_PART, MENU_SETMODE);
// 		return;
// 	}
	ns_menu::functMenu_pre(_M_REQE_READ, MENU_SETMODE);
}

/*void	reqeRead_view()
{
	if (ns_var::ml_view == 0)	return;	// запрет отображения
}*/

// ================================================================
// Read Party

void	readParty_init()
{
	ns_var::ml_adr_bd[ns_var::ml_adr_n].offSet_adr = ns_var::ml_adr_offset;
	ns_var::ml_adr_bd[ns_var::ml_adr_n].lenght_adr = 0;
	// расчет адреса
	uint32_t curAdr = ns_var::ml_adr_base + ns_var::ml_adr_offset + OFFSET_WRITE;
	// инициализация последовательной записи во флэш
	ns_user::flash->fWr_init(curAdr);
	// старт чтения с ленты
	if (ns_var::simulOn)	ns_simul::read_On();
	// ===========================
	{
		uint32_t	freeSize = 0;
		if (ns_var::s_prog == 0)
		{ // sector
			freeSize = ((uint32_t)0x1000) - ns_var::ml_adr_offset - OFFSET_WRITE;
		} 
		else
		{
			freeSize = ((uint32_t)0x10000) - ns_var::ml_adr_offset - OFFSET_WRITE;
		}
		ns_user::readData->readOn(freeSize);
	}
	// --------------------------
	scr->Clear();
	scr->SetPosition2(3, 0);
	scr->String_P(PSTR("приём данных"));
}

void	readParty_endRead()
{
	ns_user::flash->fWr_endSend();
	uint32_t	curAdr = ns_user::flash->get_wr_adr();
	uint16_t	adrOff = curAdr - ns_var::ml_adr_base - OFFSET_WRITE;
	uint16_t	lenght = adrOff - ns_var::ml_adr_offset;
	if (lenght > 0)
	{
		ns_var::ml_adr_bd[ns_var::ml_adr_n].lenght_adr = lenght;
		ns_var::ml_adr_offset = adrOff;
		ns_var::ml_adr_n++;
	}
	ns_user::readData->readOff();
	//
	if (
		(ns_var::ml_adr_n == 0)
	&&	(lenght == 0)
		)
	{
		ns_menu::functMenu_pre(_M_SCREEN1, MENU_SETMODE);
		return;
	}
	// в сектор можно писать только одну часть
	if (ns_var::s_prog == 0)
		reqeRead_save();	// сразу сохранить и выйти
	else
		ns_menu::functMenu_pre(_M_REQE_READ, MENU_SETMODE);
}

void	readParty_view()
{
	uint32_t	curAdr = ns_user::flash->get_wr_adr();
	uint16_t	adr = curAdr - ns_var::ml_adr_base - ns_var::ml_adr_offset - OFFSET_WRITE;
	uint8_t		statWork = ns_user::readData->getStatWork();
	if (statWork == ReadData::EndRead)
	{
		readParty_endRead();
		//
		return;
	}
	scr->SetPosition2(0, 1);
	scr->Hex(word_to_byte(adr).High);
	scr->Hex(word_to_byte(adr).Low);
	// ***********************
// 	scr->PutChar(' ');
// 	scr->Digit(4, ns_var::simRead_count_byte);
// 	scr->PutChar(' ');
// 	scr->Digit(4, ns_var::simRead_adr);
	scr->PutChar(' ');
// 	scr->Digit(6, ((uint32_t)curAdr) - ((uint32_t)ns_var::ml_adr_base) - ((uint32_t)OFFSET_WRITE) );
	scr->Digit(6, (ns_user::readData->getWrFreeSize()));
	//
// 	scr->PutChar(' ');
// 	scr->PutChar('0' + ns_pins::transfer_slewInc());
// 	scr->PutChar('0' + ns_pins::transfer_startStop());
// 	scr->PutChar('0' + ns_pins::transfer_spocket());
	//
// 	scr->PutChar(' ');
// 	scr->DigitZ(3, ns_user::flash->wr_head);
// 	scr->PutChar(' ');
// 	scr->DigitZ(3, ns_user::flash->wr_tail);
	//
// 	scr->SetPosition2(0, 0);
// 	scr->DigitZ(2, statWork);
}

// завершение чтения
void	readParty_k4()
{
	readParty_endRead();
	//
}
// ==============================================================
void	view_read_forward()
{
	uint8_t	lenght = 0;
	uint8_t sim;
	uint8_t fl_st = 0;
	uint16_t		adr_offset = ns_var::ml_adr_offset;

// 	uint16_t		offset_max;
// 	ns_user::flash->readArray(ns_user::flash->wr_buff, 4, adrRender(ns_var::n_prog) + 4);
// 	word_to_byte(offset_max).Low = ns_user::flash->wr_buff[2];
// 	word_to_byte(offset_max).High = ns_user::flash->wr_buff[3];

	
	ns_user::flash->serialRead_begin(ns_var::adrProg + adr_offset);
	adr_offset--;
	__delay_us(100);
	for(;lenght < 63;)
	{
		sim = ns_user::flash->serialReadByte();
		sim &= 0x7f;
		adr_offset++;
		if (fl_st == 2)	ns_var::buf_string[lenght] = sim;
		if ((sim == 0) || (sim == 0x7f))
		{
			break;	// конец данных
		}
		if ((sim == 0x0d) || (sim == 0x0a))
		{
			//lenght++;
			if (fl_st == 2)
			{
				break;
			}
			fl_st = 1;
			continue;
		}
		if (fl_st < 2)	ns_var::buf_string[lenght] = sim;
		lenght++;
		//ns_var::ml_adr_offset++;
		if (fl_st == 1)		ns_var::ml_adr_offset = adr_offset;
		fl_st = 2;
	}
	ns_user::flash->serialRead_end();
	ns_var::buf_string_lenght	= lenght;
	ns_var::buf_string_adr		= 0;
}

void	view_read_back()
{
	uint8_t sim;
	uint16_t	adr_offset = ns_var::ml_adr_offset;
	uint8_t		statFind = 0;
	uint32_t	adr_f;
	for(;adr_offset > OFFSET_WRITE; adr_offset--)
	{
		adr_f = adr_offset + ns_var::adrProg;
		ns_user::flash->readArray(ns_user::flash->wr_buff, 1, adr_f);
		sim = ns_user::flash->wr_buff[0];
		sim &= 0x7f;
		if (statFind == 0)
		{
			if ((sim != 0x0d) && (sim != 0x0a))	continue;
			statFind = 1;
		}
		if (statFind == 1)
		{
			if ((sim == 0x0d) || (sim == 0x0a))	continue;
			statFind = 2;
		}
		if (statFind == 2)
		{
			if ((sim != 0x0d) && (sim != 0x0a))	continue;
			break;
		}
	}
//	if ((sim == 0x0d) || (sim == 0x0a)) adr_offset++; // *** не обезательно
	ns_var::ml_adr_offset = adr_offset;
	view_read_forward();
}

// просмотр блока
void	viewBlock_init()
{
	scr->Clear();
	scr->String_P(PSTR("Nпр:"));
	scr->DigitZ(3, ns_var::n_prog);
	if (ns_var::n_prog >= ns_var::blockBoundary)
	{
		scr->PutChar('S');
		ns_var::ml_adr_offset_max = 4096 - 5;
		ns_var::ml_adr_offset = OFFSET_WRITE;
		ns_var::fl_viewHex = 0;
		view_read_forward();
	}
	else
	{
		scr->PutChar('B');
		ns_var::ml_adr_offset_max = 65536 - 5;
		ns_var::ml_adr_offset = 0;
		ns_var::fl_viewHex = 1;
	}
	//scr->PutChar(' ');
	view_read_forward();
	viewBlock_view_1();
}

void	viewBlock_view_2()
{
	if (ns_var::fl_viewHex != 0)	return;
	uint16_t countTmp;
	CRITICAL_SECTION
	{
		countTmp = startCount;
	}
	if (countTmp > 0)	return;
	CRITICAL_SECTION
	{
		startCount = 250;
	}
	
	scr->SetPosition2(0, 1);
	for (uint8_t i = 0; i < LENGHT_RUN_STRING; i++) scr->PutChar(' ');
	scr->SetPosition2(0, 1);
	uint8_t lenght = ns_var::buf_string_lenght;
	scr->Hex(14, ns_var::buf_string_lenght);
	if (lenght == 0)
	{
		scr->String_P( PSTR("нет данных") );
		return;
	}
	uint8_t fl_shift = (lenght >= LENGHT_RUN_STRING)?1:0;
	uint8_t fl_end = 0;
	uint8_t adr;
	uint8_t sim;
	if (fl_shift != 0)	adr = ns_var::buf_string_adr;
	else				adr = 0;
	// --
	for (uint8_t i = 0; i < LENGHT_RUN_STRING; i++)
	{
		if (fl_end == 0)
		{
			sim = ns_var::buf_string[adr];
			if ((sim == 0x00) || (sim == 0x0d) || (sim == 0x0a) || (sim == 0x7f))	fl_end = 1;
			else
			{
				if (sim >= ' ')
				{
					scr->PutChar(sim);
					adr++;
					continue;
				}
			}
		}
		scr->PutChar(' ');
	}
	if (fl_shift != 0)
	{
		if (ns_var::buf_string[ns_var::buf_string_adr] == ' ')
		{
			CRITICAL_SECTION
			{
				startCount = 800;
			}
		}
		if (fl_end != 0)
		{
			ns_var::buf_string_adr  = 0;
		}
		else	ns_var::buf_string_adr += 1;
		if (ns_var::buf_string_adr <= 1)
		{
			CRITICAL_SECTION
			{
				startCount = 2000;
			}
		}
	}
	// --
}

void	viewBlock_view()
{
	uint8_t lenght = ns_var::buf_string_lenght;
	if (lenght <= LENGHT_RUN_STRING)	return;
	viewBlock_view_2();
}

void	viewBlock_view_1()
{
	scr->SetPosition2(9, 0);
	scr->Hex(word_to_byte(ns_var::ml_adr_offset).High); // ***************************************************************************************
	scr->Hex(word_to_byte(ns_var::ml_adr_offset).Low);
	//
	if ((ns_var::n_prog < ns_var::blockBoundary) || (ns_var::fl_viewHex != 0))
	{
		scr->SetPosition2(0, 1);
		ns_user::flash->readArray(ns_user::flash->wr_buff, 5, ns_var::adrProg + ns_var::ml_adr_offset);
		for (uint8_t i = 0; i < 5; i++)
		{
			if ( (ns_var::ml_adr_offset & 0x01) == (i & 0x01))
			{
				scr->PutChar('.');
			}
			else
			{
				scr->PutChar(' ');
			}
			scr->Hex(ns_user::flash->wr_buff[i]);
		}
		scr->PutChar(' ');
	}
	else
	{
		viewBlock_view_2();
	}
}

void	viewBlock_k1()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

void	viewBlock_k2()
{
	if (ns_var::ml_adr_offset > 0)
	{
// 		ns_var::ml_adr_offset--;
// 		//view_read_forward();
// 		viewBlock_view_1();
		if ((ns_var::n_prog < ns_var::blockBoundary) || (ns_var::fl_viewHex != 0))
		{
			ns_var::ml_adr_offset--;
			viewBlock_view_1();
		}
		else
		{
			CRITICAL_SECTION
			{
				startCount = 0;
			}
			view_read_back();
			viewBlock_view_1();
		}
	}
}

void	viewBlock_k3()
{
	if ((ns_var::ml_adr_offset + 1) < ns_var::ml_adr_offset_max)
	{
		if ((ns_var::n_prog < ns_var::blockBoundary) || (ns_var::fl_viewHex != 0))
		{
			ns_var::ml_adr_offset++;
			viewBlock_view_1();
		}
		else
		{
			CRITICAL_SECTION
			{
				startCount = 0;
			}
			ns_var::ml_adr_offset += ns_var::buf_string_lenght;
			view_read_forward();
			viewBlock_view_1();
		}
	}
}

void	viewBlock_k4()
{
	if (ns_var::fl_viewHex == 0)	ns_var::fl_viewHex = 1;
	else
	{
		ns_var::fl_viewHex = 0;
		view_read_forward();
	}
	viewBlock_view_1();
}
// ====================================================================
// подготовка к запуску
// -------------------- REQE WRITE ------------------------------------
void	reqeWrite_begin()
{
	// настройка портов
	ns_user::writeData->initPorts();
	//
	ns_var::ml_adr_base = adrRender(ns_var::n_prog);	// базовый адрес блока/сектора
	// количество частей
	ns_user::flash->readArray(ns_user::flash->wr_buff, 2, ns_var::ml_adr_base);
	ns_var::ml_adr_n_max = ns_user::flash->wr_buff[0];
#ifdef	__MENU_DEBUG__
	ns_var::ml_adr_n_max = 2;
#endif
	if ( (ns_var::ml_adr_n_max == 0xff) || (ns_var::ml_adr_n_max == 0) )
	{
		scr->Clear();
		scr->String_P(PSTR("нет данных"));
		__delay_ms(2000);
		ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
		return;
	}
	if (ns_var::ml_adr_n_max > ns_var::ml_adr_bd_max)
	{
		scr->Clear();
		scr->String_P(PSTR("не кор.данные"));
		__delay_ms(2000);
		ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
		return;
	}
	// -------------------------------------------------------------------------------------------------------
	// загрузка бд частей программы
	// ml_adr_n_max - количество частей в программе
	// бд: 2 байта адрес, 2 байта длина на каждую часть
	// ml_adr_base + 4 адрес  начала БД
	ns_user::flash->readArray(ns_user::flash->wr_buff, (ns_var::ml_adr_n_max * 4), (ns_var::ml_adr_base + 4));
	uint8_t adr_flash; // адрес в БД
	uint32_t	adr_party, lenght_party;	// адрес текущей части и ее длина
	for (uint8_t i = 0; i < ns_var::ml_adr_n_max; i++)
	{
		adr_flash = i * 4;
		adr_party = lenght_party = 0;
		word_to_byte(adr_party).Low			= ns_user::flash->wr_buff[adr_flash + 0];
		word_to_byte(adr_party).High		= ns_user::flash->wr_buff[adr_flash + 1];
		word_to_byte(lenght_party).Low		= ns_user::flash->wr_buff[adr_flash + 2];
		word_to_byte(lenght_party).High		= ns_user::flash->wr_buff[adr_flash + 3];
		// **********************************************************************************************************************************
#ifdef	__MENU_DEBUG__
		adr_party = 0;
		lenght_party = 60000;
#endif
		//
		ns_var::ml_adr_bd[i].offSet_adr		= adr_party;
		ns_var::ml_adr_bd[i].lenght_adr		= lenght_party;
	}
	// -------------------------------------------------------------------------------------------------------
	// установка начальной текущей части
	ns_var::ml_adr_n = 0;
	// т.к в программе только одна часть или первая, то сразу включить передачу
	ns_menu::functMenu_pre(_M_WRITE_PART, MENU_SETMODE);
}

void	reqeWrite_init()
{
	scr->Clear();
	scr->String_P(PSTR("часть "));
	scr->Digit(2, ns_var::ml_adr_n + 1);
	scr->String_P(PSTR(" из "));
	scr->Digit(2, ns_var::ml_adr_n_max);
	scr->SetPosition2(0, 1);
	scr->String_P(PSTR("дальше ?"));
	startCount = 5 * 1000;
}

void	reqeWrite_k2()
{
	scr->Clear();
	scr->String_P(PSTR("отмена"));
	__delay_ms(1500);
	ns_menu::functMenu_pre(_M_SCREEN1, MENU_SETMODE);
}

void	reqeWrite_k3()
{	// дальше
	ns_menu::functMenu_pre(_M_WRITE_PART, MENU_SETMODE);
}

void	reqeWrite_check()
{
	uint16_t count_tmp;
	CRITICAL_SECTION
	{
		count_tmp = startCount;
	}
	scr->Digit(scr->SetPosition(14, 1), 2, count_tmp / 1000);
	if (count_tmp == 0)
	{
		ns_menu::functMenu_pre(_M_WRITE_PART, MENU_SETMODE);
	}
}

// ====================================================================
// -------------------- WRITE PARTY -----------------------------------

void	writeParty_init()
{
	// адрес и длина текущей части
	ns_var::sendAdr		=	adrRender(ns_var::n_prog) + ns_var::ml_adr_bd[ns_var::ml_adr_n].offSet_adr + OFFSET_WRITE;
	ns_var::sendLenght	=	ns_var::ml_adr_bd[ns_var::ml_adr_n].lenght_adr;
	// установка начального индекса для отображения отправки
	// при включении режима отправки индекс обнуляется
	// ------------------------------------------------------------------
	// настройка последовательного чтения с флеш
	ns_user::flash->fRd_init(ns_var::sendAdr, ns_var::sendLenght);
	// ------------------------------------------------------------------
	// сброс модуля передачи
	ns_user::writeData->sendOff();
	// ------------------------------------------------------------------
	ns_user::writeData->initPorts();
	//
	// установка режима отправки
	ns_user::writeData->sendOn();
	// ------------------------------------------------------------------
	scr->Clear();
	scr->String_P(PSTR("отправка "));
	scr->DigitZ(2, ns_var::ml_adr_n + 1);
	scr->String_P(PSTR(" части:"));
}

void	writeParty_end()
{
	if ( (ns_var::ml_adr_n_max > 1) && (ns_var::ml_adr_n < ns_var::ml_adr_n_max) )
	{
		// ожидание разрешения следущей части
		ns_menu::functMenu_pre(_M_REQE_WRITE, MENU_SETMODE);
	}
	else
	{
		if (ns_var::simulOn == 0)
		{
			ns_menu::functMenu_pre(_M_SCREEN1, MENU_SETMODE);
		}
		else
		{
			// проверка на  флаг ошибки при симуляции
			scr->Clear();
			if (ns_user::writeData->error_sim == 0)
			{	// символы/коды совпадают
				scr->String_P( PSTR("Коды совпадают") );
			}
			else
			{	// символы/коды не совпадают
				scr->String_P( PSTR("Коды не совпадают") );
				scr->SetPosition2(0, 1);
				scr->Hex(word_to_byte(ns_user::writeData->error_sim_adr).High);
				scr->Hex(word_to_byte(ns_user::writeData->error_sim_adr).Low);
				scr->PutChar(' ');
				scr->Hex(ns_user::writeData->error_sim_dat);
			}
			ns_menu::functMenu_aft(_M_WT_SCR1, MENU_SETMODE);
		}
	}
}

void	writeParty_view()
{
	// количество отправленных байт
	scr->SetPosition2(0, 1);
	scr->Digit(5, ns_user::writeData->getSendCountByte());
	scr->String_P(PSTR(" из "));
	scr->Digit(5, ns_var::sendLenght);
	// проверка окончания передачи
	uint8_t	stat = ns_user::writeData->getStatusWork();
	if (stat == ns_user::writeData->sendEnd)
	{
		// сброс статуса модуля передачи
		ns_user::writeData->sendOff();
		// следущая часть
		ns_var::ml_adr_n++;
		writeParty_end();
		return;
	}
// 	scr->Hex(17, PIND);
// 	scr->Digit(20, 3, ns_user::writeData->getStatusWork());
}

void	writeParty_k1()
{	// отмена
	// сброс статуса модуля передачи
	ns_user::writeData->sendOff();
	// сброс последовательного чтения
	ns_user::flash->fRd_end();
	scr->Clear();
	scr->String_P(PSTR("отмена передачи"));
	__delay_ms(3000);
	ns_menu::functMenu_pre(_M_SCREEN1, MENU_SETMODE);
}

// ==================================================================
void	plus5_view()
{
	if (ns_var::flag_plus5_m == 0)
	{
		scr->String_P(7, PSTR("Off"));
	}
	else
	{
		ns_var::flag_plus5_m = 1;
		scr->String_P(7, PSTR("On "));
	}
}

void	plus5_init()
{
	scr->Clear();
	scr->String_P(PSTR("R+5v : "));
	ns_var::flag_plus5_m = eeprom_read_byte(&ns_var::flag_plus5);
	plus5_view();
}

void	plus5_k1()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

void	plus5_k2()
{
	ns_var::flag_plus5_m = 0;
	plus5_view();
}

void	plus5_k3()
{
	ns_var::flag_plus5_m = 1;
	plus5_view();
}

void	plus5_k4()
{
	eeprom_update_byte(&ns_var::flag_plus5, ns_var::flag_plus5_m);
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

// ----------------------------------------------------

void	eot_view()
{
	if (ns_var::flag_eot_m == 0)
	{
		scr->String_P(13, PSTR("Off"));
	} 
	else
	{
		ns_var::flag_eot_m = 1;
		scr->String_P(13, PSTR("On "));
	}
}

void	eot_init()
{
	scr->Clear();
	scr->String_P(PSTR("eot or rhu : "));
	ns_var::flag_eot_m = eeprom_read_byte(&ns_var::flag_eot);
	eot_view();
}

void	eot_k1()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

void	eot_k2()
{
// 	ns_var::flag_eot_m = 0;
// 	eot_view();
}

void	eot_k3()
{
// 	ns_var::flag_eot_m = 1;
// 	eot_view();
}

void	eot_k4()
{
// 	eeprom_update_byte(&ns_var::flag_eot, ns_var::flag_eot_m);
	uint32_t adr = adrRender(ns_var::n_prog); // + OFFSET_WRITE;
	uint8_t cod;
	uint16_t lenght_array = sizeof(ms);
	/*for (uint8_t i = 0; i < 92; i++)
	{
		cod = pgm_read_byte(&ms[i]);
		cod = odd_plus_7bit(cod);
		ns_user::flash->wr_buff[i] = cod;
	}*/
	ns_user::flash->fWr_init(adr + OFFSET_WRITE);
	for (uint16_t i = 0; i < lenght_array; i++)
	{
		cod = pgm_read_byte(&ms[i]);
		cod = odd_plus_7bit(cod);
		ns_user::flash->fWr_dataSend(cod);
	}
	ns_user::flash->fWr_endSend();
	__delay_ms(500);
	ns_user::flash->writeArray(ns_user::flash->wr_buff, 92, adr + OFFSET_WRITE);
	ns_user::flash->wr_buff[0] = 1;
	ns_user::flash->wr_buff[1] = 0xff;
	ns_user::flash->wr_buff[2] = 0xff;
	ns_user::flash->wr_buff[3] = 0xff;
	ns_user::flash->wr_buff[4] = 0;
	ns_user::flash->wr_buff[5] = 0;
	ns_user::flash->wr_buff[6] = word_to_byte(lenght_array).High;
	ns_user::flash->wr_buff[7] = word_to_byte(lenght_array).Low;
	ns_user::flash->wr_buff[8] = 0xff;
	ns_user::flash->wr_buff[9] = 0xff;
	ns_user::flash->writeArray(ns_user::flash->wr_buff, 10, adr);
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}
// ------------------------------
void	pins_init()
{
	scr->Clear();
	scr->SetPosition2(0, 1);
	scr->String_P(PSTR("76543210"));
	scr->String_P(PSTR("sSdrilSE"));
	ns_var::pins_uk = 0;
}

void	pins_view()
{
	uint8_t	dat = ns_pins::transfer_data();
	switch(ns_var::pins_uk)
	{
		case 0:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit7);
		break;
		case 1:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit6);
		break;
		case 2:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit5);
		break;
		case 3:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit4);
		break;
		case 4:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit3);
		break;
		case 5:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit2);
		break;
		case 6:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit1);
		break;
		case 7:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + bit_is_byte(dat).bit0);
		break;
		//
		case 8:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_sprocket());
		break;
		case 9:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_strobe());
		break;
		case 14:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_startStop());
		break;
		case 15:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_eotOrRhu());
		break;

	}
	ns_var::pins_uk++;
	if (ns_var::pins_uk >= LENGHT_RUN_STRING)	ns_var::pins_uk = 0;
}

void	pins_k1()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

// ------------------------------------------------------
void wt_scr1_go()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}
// ------------------------------------------------------

#endif // CONF_MENU

