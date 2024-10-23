/* 
* mainUser.cpp
*
* Created: 23.04.2024 13:24:32
* Author: User
*/


#include "mainUser.h"

#include <avr/eeprom.h>

#include "core/core.h"
#include "user/var.h"
#include "user/line/Simulyation.h"
#include "user/line/StartReady.h"



namespace	ns_user
{
	Spi_Hard	*spi_h;
	MX25L8005	*flash;
	ReadData	*readData;
	WriteData	*writeData;
}

void	ns_user::init()
{
	if (ns_var::simulOn)
	{
		ns_simul::read_init();
	}
	
	// работа с spi
	spi_h = Spi_Hard::init();
	// работа с флеш mx8005
	flash = MX25L8005::init();
	// алгоритм для чтения перфоленты
	readData = ReadData::init();
	//
	writeData = WriteData::init();
	
	// настроить порты на чтение с перфоленты
	readData->initPorts();
	
	// длина программы минус
	ns_var::subOutLenght		= eeprom_read_byte(&ns_var::subOutLenght_e);
	
	// режим работы с сигналом left/right
	ns_var::leftRight_stat		= eeprom_read_byte(&ns_var::leftRight_stat_e);
	
	// настройка работы сигнала Start/Stop по прерыванию
	 StartReady::init();
	
}
