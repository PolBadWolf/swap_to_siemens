/* 
* mainUser.cpp
*
* Created: 23.04.2024 13:24:32
* Author: User
*/


#include "mainUser.h"

#include "core/core.h"
#include "user/var.h"
#include "user/line/Simulyation.h"
#include <avr/eeprom.h>



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
	
	// кофиценты
	ns_var::safeDelay_minINT	= eeprom_read_byte(&ns_var::safeDelay_minINT_e);
	ns_var::safeDelay_minSD		= eeprom_read_byte(&ns_var::safeDelay_minSD_e);
	
	ns_var::safeDelay_plsINT	= eeprom_read_byte(&ns_var::safeDelay_plsINT_e);
	ns_var::safeDelay_plsFD		= eeprom_read_byte(&ns_var::safeDelay_plsFD_e);
	ns_var::safeDelay_plsSD		= eeprom_read_byte(&ns_var::safeDelay_plsSD_e);
}
