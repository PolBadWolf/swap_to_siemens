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
	
// 	bufferRW[0] = 0x12;
// 	bufferRW[1] = 0x58;
// 	bufferRW[2] = 0x9b;
// 	bufferRW[3] = 0xf4;
// 	flash->writeArray(bufferRW, 4, 0x08);
}
