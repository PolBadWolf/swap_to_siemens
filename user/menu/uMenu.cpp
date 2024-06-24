/* 
* uMenu.cpp
*
* Created: 23.04.2024 10:26:20
* Author: User
*/

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

#include "user/line/signal_pin.h"
#include "system/indication/Lcd_hard.h"

// #define __MENU_DEBUG__

void	m1_init()
{
	key->autoRepeatOn();
	scr->Clear();
	scr->String_P(PSTR("ADR = "));
	m1_view();
}

/*namespace	ns_user
{
	extern	uint8_t		bufferRW[];
// 	extern	uint8_t		result;
}*/

uint32_t	adr = 0x2000;

// uint16_t	idf = 0;

void	m1_view()
{
	scr->Hex(6, selectByte(adr, 2));
	scr->Hex(8, selectByte(adr, 1));
	scr->Hex(10, selectByte(adr, 0));

// 	scr->Hex(12, ns_user::result);
// 	for(uint8_t i = 0; i < 8; i++)
// 	{
// 		scr->Hex(25 + (i * 3), ns_user::bufferRW[adr + i]);
// 	}
// 	idf = ns_user::flash->getID();
// 	scr->Hex(11, word_to_byte(idf).High);
// 	scr->Hex(13, word_to_byte(idf).Low);
	uint8_t	stat;
	scr->Hex(16, ns_user::flash->getStatus());
	stat = ns_user::flash->readArray(ns_user::flash->wr_buff, 8, adr);
	if (stat != 0)
	{
		scr->String_P(scr->SetPosition(0, 1), PSTR("ошибка связи"));
	}
	else
	{
		for (uint8_t i = 0; i < 8; i++)
		{
			scr->Hex(25 + (i * 3), ns_user::flash->wr_buff[i]);
		}
	}
}

void	m1_key2()
{
// 	adr = (adr & 0xfff8) - 8;
	adr--;
	m1_view();
}

void	m1_key3()
{
// 	adr = (adr & 0xfff8) + 8;
	adr++;
	m1_view();
}

void	m1_key1()
{
	ns_user::flash->eraseSector(adr);
	m1_view();
}

void	m1_key4()
{
	uint8_t	stat = 0;
	ns_user::flash->wr_buff[0] = 0x55;
	ns_user::flash->wr_buff[1] = 0xaa;
	ns_user::flash->wr_buff[2] = 0x19;
	ns_user::flash->wr_buff[3] = 0xe6;
	do 
	{
		stat = ns_user::flash->readArray(&ns_user::flash->wr_buff[4], 200, 0x1f00);
		if (stat != 0)	break;
		
		stat = ns_user::flash->writeArray(ns_user::flash->wr_buff, 200, adr);
		if (stat != 0)	break;
	} while (false);
	if (stat == 0)
	{
		m1_view();
	}
	else
	{
		scr->String_P(scr->SetPosition(0, 1), PSTR("ошибка связи"));
	}
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
	ns_var::n_prog		= 0;
	ns_var::adrProg		= adrRender(ns_var::n_prog);
	ns_var::mxMod		= LIST_MOD_load;
	//
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
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

void	screen1_vmem(uint32_t adr)
{
	uint8_t	sb[5];
	ns_user::flash->readArray(sb, 5, adr);
	scr->SetPosition2(0, 1);
	for (uint8_t i = 0; i < 5; i++)
	{
		scr->PutChar(' ');
		scr->Hex(sb[i]);
	}
}

void	screen1_view()
{
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
				if (cx < 500)	continue;
				cx = 0;
				if (id == ns_user::flash->idDev)
				{
					ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
					return;
				}
			}
		}
	} 
	else
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

void	screen1_init()
{
	// настройка портов на чтение
	ns_user::readData->initPorts();
	// -------------------------------------
	ns_var::mxMod = 0;
	scr->Clear();
	key->autoRepeatOn();
	scr->String_P(PSTR("Nпр:"));
	screen1_view();
}

void	screen1_k2()
{
	if (ns_var::n_prog > 0)
	{
		ns_var::n_prog--;
		ns_var::adrProg = adrRender(ns_var::n_prog);
		ns_var::mxMod = 0;
		screen1_view();
	}
}

void	screen1_k3()
{
	if (ns_var::n_prog < progMax)
	{
		ns_var::n_prog++;
		ns_var::adrProg = adrRender(ns_var::n_prog);
		ns_var::mxMod = 0;
		screen1_view();
	}
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
		case LIST_MOD_save:		reqeWrite_begin();
		break;
		case LIST_MOD_clr:		ns_menu::functMenu_aft(_M_CLEAR, MENU_SETMODE);
		break;
		case LIST_MOD_copy:		ns_menu::functMenu_aft(_M_COPY, MENU_SETMODE);
		break;
		case LIST_MOD_load:		reqeRead_begin();
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
	// если принята хоть одна часть, то сохранить.
	if (ns_var::ml_adr_n > 0)
	{
		ns_user::flash->wr_buff[0] = ns_var::ml_adr_n;
		ns_user::flash->wr_buff[1] = 0xff;
		ns_user::flash->wr_buff[2] = 0xff;
		ns_user::flash->wr_buff[3] = 0xff;
		uint16_t curAdr, curLenght;
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
		curLenght = (ns_var::ml_adr_n + 1) * 4;
		ns_user::flash->writeArray(ns_user::flash->wr_buff, curLenght, ns_var::ml_adr_base);
	}
	//
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
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
	ns_user::readData->readOn();
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
	if (ns_var::s_prog == 0)	reqeRead_save();	// сразу сохранить и выйти
	else						ns_menu::functMenu_pre(_M_REQE_READ, MENU_SETMODE);
}

void	readParty_view()
{
	uint32_t	curAdr = ns_user::flash->get_wr_adr();
	uint16_t	adr = curAdr - ns_var::ml_adr_base - ns_var::ml_adr_offset - OFFSET_WRITE;
	if (ns_user::readData->getStatWork() == ReadData::EndRead)
	{
		readParty_endRead();
		//
		return;
	}
	scr->SetPosition2(0, 1);
	scr->Hex(word_to_byte(adr).High);
	scr->Hex(word_to_byte(adr).Low);
	//
// 	scr->PutChar(' ');
// 	scr->PutChar('0' + ns_pins::transfer_slewInc());
// 	scr->PutChar('0' + ns_pins::transfer_startStop());
// 	scr->PutChar('0' + ns_pins::transfer_spocket());
	//
	scr->PutChar(' ');
	scr->DigitZ(3, ns_user::flash->wr_head);
	scr->PutChar(' ');
	scr->DigitZ(3, ns_user::flash->wr_tail);
	//
	scr->SetPosition2(0, 0);
	scr->DigitZ(2, ns_user::readData->getStatWork());
}

// завершение чтения
void	readParty_k4()
{
	readParty_endRead();
	//
}
// ==============================================================
// просмотр блока
void	viewBlock_init()
{
	ns_var::ml_adr_offset = 0;
	scr->Clear();
	scr->String_P(PSTR("Nпр:"));
	scr->DigitZ(3, ns_var::n_prog);
	if (ns_var::s_prog == 0)
	{
		scr->PutChar('S');
		ns_var::ml_adr_offset_max = 4096 - 5;
	}
	else
	{
		scr->PutChar('B');
		ns_var::ml_adr_offset_max = 65536 - 5;
	}
	scr->PutChar(' ');
	viewBlock_view();
}

void	viewBlock_view()
{
	scr->SetPosition2(9, 0);
	scr->Hex(word_to_byte(ns_var::ml_adr_offset).High);
	scr->Hex(word_to_byte(ns_var::ml_adr_offset).Low);
	//
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
}

void	viewBlock_k1()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

void	viewBlock_k2()
{
	if (ns_var::ml_adr_offset > 0)
	{
		ns_var::ml_adr_offset--;
		viewBlock_view();
	}
}

void	viewBlock_k3()
{
	if ((ns_var::ml_adr_offset + 1) < ns_var::ml_adr_offset_max)
	{
		ns_var::ml_adr_offset++;
		viewBlock_view();
	}
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
	// если только одна чать в программе, то в главное меню
	if (ns_var::ml_adr_n_max == 1)
	{
		ns_menu::functMenu_pre(_M_SCREEN1, MENU_SETMODE);
		return;
	}
	// передана послядняя часть - завершено
	if (ns_var::ml_adr_n >= ns_var::ml_adr_n_max)
	{
		ns_menu::functMenu_pre(_M_SCREEN1, MENU_SETMODE);
		return;
	}
	// ожидание разрешения следущей части
	ns_menu::functMenu_pre(_M_REQE_WRITE, MENU_SETMODE);
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
	ns_var::flag_eot_m = 0;
	eot_view();
}

void	eot_k3()
{
	ns_var::flag_eot_m = 1;
	eot_view();
}

void	eot_k4()
{
	eeprom_update_byte(&ns_var::flag_eot, ns_var::flag_eot_m);
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
		case 10:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_dataEnable());
		break;
		case 11:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_readyBusy());
		break;
		case 12:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_slewInc());
		break;
		case 13:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_leftRight());
		break;
		case 14:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_startStop());
		break;
		case 15:
		scr->PutChar(scr->SetPosition(ns_var::pins_uk, 0), '0' + ns_pins::transfer_eotOrRhu());
		break;

	}
	ns_var::pins_uk++;
	if (ns_var::pins_uk >= 16)	ns_var::pins_uk = 0;
}

void	pins_k1()
{
	ns_menu::functMenu_aft(_M_SCREEN1, MENU_SETMODE);
}

#endif // CONF_MENU

