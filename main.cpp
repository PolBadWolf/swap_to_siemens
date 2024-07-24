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
void	read_string(uint32_t adr);
void	viewBlock_view_1();
void	viewBlock_init();
void	view_read_forward();
void	view_read_back();
void	sendParty_init();

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

	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		Core::idleCycle();
    }
}

