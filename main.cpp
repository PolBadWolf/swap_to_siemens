/*
 * test_1.cpp
 *
 * Created: 10.04.2024 11:23:10
 * Author : User
 */ 

#include "core/core.h"

// #include "system/communication/spi/Spi_Hard.h"
#include "user\mainUser.h"
#include "user\system\menuUser.h"
#include "system\menu\menu.h"
#include "user\var.h"

#include "user/line/signal_pin.h"
#include "indication/Lcd_hard.h"
#include <avr/pgmspace.h>
#include "user/menu/uMenu.h"
#include "core/delay.h"


// Spi_Hard	*tt;
uint8_t	yy, x;
uint8_t	a1 = 1, a2 = 2, a3= 3, a4 = 4;


void fgh()
{
	asm volatile(
	"pop	%0		\n\t"
	"pop	%1		\n\t"
	"pop	%2		\n\t"
	"pop	%3		\n\t"
	"push	%3		\n\t"
	"push	%2		\n\t"
	"push	%1		\n\t"
	"push	%0		\n\t"
	:	"=r" (a1), "=r" (a2), "=r" (a3), "=r" (a4)
	);
}

	void	(*ochered[6][6])() =
	{
		fgh
	};
	
// void	read_string(uint32_t adr);
// void	viewBlock_view_1();
// void	viewBlock_init();
// void	view_read_forward();
// void	view_read_back();
// void	sendParty_init();

// void	screen1_k1();

int main(void)
{
// 	ochered[0] = ns_user::flash->mx_irq;
// 	ochered[1] = fgh;
// 	ochered[2] = (void (*)())main;
	//ochered[0][0]();
	Core::init();

// 	eot_k4();
// ns_user::writeData->timerFast();
// ns_user::writeData->timerFast();
// ns_var::simulOn = 1;
// ns_user::writeData->sendOn();
// ns_user::writeData->timerFast();
// 	ns_var::mxMod = 0;
// 	do 
// 	{
// 		screen1_k1();
// 	} while (true);

	/*ns_var::buf_string_lenght = 57;
	ns_var::buf_string[ 0] = '(';
	ns_var::buf_string[ 1] = 'R';
	ns_var::buf_string[ 2] = 'x';
	ns_var::buf_string[ 3] = '=';
	ns_var::buf_string[ 4] = '5';
	ns_var::buf_string[ 5] = '7';
	ns_var::buf_string[ 6] = '5';
	ns_var::buf_string[ 7] = ' ';
	ns_var::buf_string[ 8] = 'O';
	ns_var::buf_string[ 9] = 'T';

	ns_var::buf_string[10] = 'T';
	ns_var::buf_string[11] = 'M';
	ns_var::buf_string[12] = '-';
	ns_var::buf_string[13] = '3';
	ns_var::buf_string[14] = '2';
	ns_var::buf_string[15] = '4';
	ns_var::buf_string[16] = '/';
	ns_var::buf_string[17] = '1';
	ns_var::buf_string[18] = '0';
	ns_var::buf_string[19] = ' ';

	ns_var::buf_string[20] = 'K';
	ns_var::buf_string[21] = '1';
	ns_var::buf_string[22] = '-';
	ns_var::buf_string[23] = '5';
	ns_var::buf_string[24] = '2';
	ns_var::buf_string[25] = ' ';
	ns_var::buf_string[26] = 'P';
	ns_var::buf_string[27] = ':';
	ns_var::buf_string[28] = ' ';
	ns_var::buf_string[29] = 'P';

	ns_var::buf_string[30] = '?';
	ns_var::buf_string[31] = 'P';
	ns_var::buf_string[32] = '0';
	ns_var::buf_string[33] = 'Q';
	ns_var::buf_string[34] = 2;
	ns_var::buf_string[35] = 'Q';
	ns_var::buf_string[36] = 0;
	ns_var::buf_string[37] = 'P';
	ns_var::buf_string[38] = '>';
	ns_var::buf_string[39] = 'P';

	ns_var::buf_string[40] = '=';
	ns_var::buf_string[41] = 'Q';
	ns_var::buf_string[42] = 3;
	ns_var::buf_string[43] = ' ';
	ns_var::buf_string[44] = '2';
	ns_var::buf_string[45] = '8';
	ns_var::buf_string[46] = '/';
	ns_var::buf_string[47] = '0';
	ns_var::buf_string[48] = '2';
	ns_var::buf_string[49] = '/';

	ns_var::buf_string[50] = '2';
	ns_var::buf_string[51] = '0';
	ns_var::buf_string[52] = '0';
	ns_var::buf_string[53] = '8';
	ns_var::buf_string[54] = ')';
	ns_var::buf_string[55] = 13;
	ns_var::buf_string[56] = 10;
	ns_var::buf_string[57] = 0;*/

	sei();
	
    /* Replace with your application code */
// 	__delay_ms(500);
// 	viewBlock_k3();
    while (1) 
    {
// 		menuUser_1Milisec();
// 		viewBlock_view();
		Core::idleCycle();
    }
}

