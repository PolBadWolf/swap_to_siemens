/* 
* mainUser.h
*
* Created: 23.04.2024 13:24:32
* Author: User
*/


#ifndef __MAINUSER_H__
#define __MAINUSER_H__

#include "communication/spi/Spi_Hard.h"
#include "user/mx25L8005/MX25L8005.h"
#include "user/line/ReadData.h"
#include "user/line/WriteData.h"

namespace	ns_user
{
	void	init();
	extern	Spi_Hard	*spi_h;
	extern	MX25L8005	*flash;
	extern	ReadData	*readData;
	extern	WriteData	*writeData;
}



#endif //__MAINUSER_H__
