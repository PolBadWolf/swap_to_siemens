/* 
* core.h
*
* Created: 10.04.2024 11:34:10
* Author: User
*/


#ifndef __CORE_H__
#define __CORE_H__

#include "core/core_def.h"
#include "config_sys.h"

#ifdef CONF_LCD
	#include "indication/Lcd.h"
#endif // CONF_LCD

#ifdef CONF_KEY4
	#include "keyboard/Key.h"
#endif // CONF_KEY4

#ifdef CONF_RS232
	#include "communication/Serial.h"
#endif // CONF_RS232

#ifdef CONF_SPI_SOFT
	#include "communication/SPI.h"
#endif // CONF_SPI_SOFT

namespace	Core
{
	void	init();
	void	idleCycle();
	extern	uint16_t	spaceCycle;
}

#ifdef CONF_LCD
	extern	Lcd	*scr;
#endif // CONF_LCD

#ifdef CONF_KEY4
	extern	Key	*key;
#endif // CONF_KEY4

#ifdef CONF_RS232
	extern	Serial		*serial;
#endif // CONF_RS232

#ifdef CONF_SPI_SOFT
	extern	Spi		*spi;
#endif // CONF_SPI_SOFT


#endif //__CORE_H__
