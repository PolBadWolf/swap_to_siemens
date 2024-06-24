/* 
* Sd.cpp
*
* Created: 23.04.2024 13:19:28
* Author: User
*/


#include "Sd.h"
#include "core/core.h"

// default constructor
Sd::Sd()
{
} //Sd

// default destructor
Sd::~Sd()
{
} //~Sd

uint8_t	Sd::sendCommand(uint8_t	dt0, uint8_t dt1, uint8_t dt2, uint8_t dt3, uint8_t dt4, uint8_t dt5)
{
	uint8_t	result;
	int32_t cnt;
	spi->sendByte(dt0);
	spi->sendByte(dt1);
	spi->sendByte(dt2);
	spi->sendByte(dt3);
	spi->sendByte(dt4);
	spi->sendByte(dt5);
	cnt = 0;
	do 
	{
		result = spi->readByte();
		cnt++;
	} while ( ((result & 0x80) != 0x00) && (cnt < 0xffff) );
	return	result;
}

uint8_t	Sd::init()
{
	uint8_t	i, temp;
	int32_t	cnt;
	spi->setSS(true);
	// 80 импульсов (не менее 74) Даташит стр 94
	for (i = 0; i < 20; i++)
	{
		spi->sendByte(0xff);
	}
	// опускаем SS
	spi->setSS(false);
	// CMD0 Даташит стр 102 и 96
	temp = sendCommand(0x40, 0x00, 0x00, 0x00, 0x00, 0x95);
	// Выйти, если ответ не 0х01 (результат
	
	scr->Hex(18, temp);
	
	if (temp != 0x01)	return	3;
	spi->sendByte(0xff);
	
	cnt = 0;
	do 
	{
		temp = sendCommand(0x41, 0x00, 0x00, 0x00, 0x00, 0x95);
		spi->sendByte(0xff);
		cnt++;
	} while ( (temp != 0x00) && (cnt < 0xffff) );
	if (cnt >= 0xffff)	return	4;
	return	0;
}

uint8_t	Sd::write_Block(uint8_t	*bf, uint8_t dt1, uint8_t dt2, uint8_t dt3, uint8_t dt4)
{
	uint8_t	result, i;
	int32_t	cnt;
	// CMD24 даташит стр 51 и 97-98
	result = sendCommand(0x58, dt1, dt2, dt3, dt4, 0x95);
	if (result != 0x00)	return	6;
	//
	spi->sendByte(0xff);
	spi->sendByte(0xfe);
	for (i = 0; i < 512; i++)
	{
		spi->sendByte(bf[i]);
	}
	spi->sendByte(0xff);
	spi->sendByte(0xff);
	result = spi->readByte();
	// Выйти, если результат не 0x05 (Даташит стр 111)
	if ( (result & 0x05) != 0x05)	return	6;
	//
	cnt = 0;
	do 
	{
		result = spi->readByte();
		cnt++;
	} while ( (result != 0xff) && (cnt < 0xffff) );
	//
	if (cnt >= 0xffff)	return	6;
	return	0;
}

uint8_t	Sd::read_Block (uint8_t	*bf, uint8_t dt1, uint8_t dt2, uint8_t dt3, uint8_t dt4)
{
	uint8_t	result, i;
	int32_t	cnt;
	// CMD17 даташит стр 50 и 96
	result = sendCommand(0x51, dt1, dt2, dt3, dt4, 0x95);
	// Выйти, если результат не 0x00
	if (result != 0x00)	return	5;
	spi->sendByte(0xff);
	// Ждем начала блока
	cnt = 0;
	do 
	{
		result = spi->readByte();
		cnt++;
	} while ( (result != 0xfe) && (cnt < 0xffff) );
	//
	if (cnt >= 0xffff)	return	5;
	// получаем байты блока из шины в буфер
	for (i = 0; i < 512; i++)
	{
		bf[i] = spi->readByte();
	}
	// Получаем контрольную сумму
	spi->readByte();
	spi->readByte();
	return	0;
}


