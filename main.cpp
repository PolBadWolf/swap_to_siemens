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
void	writeParty_init();

int main(void)
{
// 	ochered[0] = ns_user::flash->mx_irq;
// 	ochered[1] = fgh;
// 	ochered[2] = (void (*)())main;
	//ochered[0][0]();
	Core::init();

// writeParty_init();
// for(;;)
// {
// 	ns_user::writeData->timerFast_irq();
// }


// 	__delay_ms(1000);
//  	eot_k4();
// 	viewBlock_init();
// 	view_read_back();
// 	viewBlock_view();
// 	ns_var::ml_adr_offset += ns_var::buf_string_lenght;
// 	view_read_forward();
// 	viewBlock_view_1();

// 	yy = ns_user::readData->checkErrorParty(0x82);
// 	scr->PutChar('0' + yy);
// 	yy = ns_user::readData->checkErrorParty(0x02);
// 	scr->PutChar('0' + yy);
// 	yy = ns_user::readData->checkErrorParty(0x8d);
// 	scr->PutChar('0' + yy);
// 	yy = ns_user::readData->checkErrorParty(0x8a);
// 	scr->PutChar('0' + yy);
// 	yy = ns_user::readData->checkErrorParty(0x8b);
// 	scr->PutChar('0' + yy);

// ns_user::readData->readOn();
// for(;;)	
// ns_user::readData->timerFast();	
	
// 	ns_user::flash->fWr_init(0);
// // 	ns_user::flash->fWr_dataSend(0x33);
// 	ns_user::flash->mx_irq();
// 	ns_user::flash->fWr_endSend();

	//ns_user::writeData->modeDelay(WriteData::phaze1_1, 2);


// 	ns_user::flash->fRd_init(0x2000, 0x10);
// 	ns_user::writeData->sendOn();
// 	while (ns_user::writeData->getStatusWork() != WriteData::sendEnd)
// 	{
// 		ns_user::writeData->timerFast();
// 	}
/*
scr->Clear();
scr->SetPosition2(0, 1);
scr->String_P(PSTR("76543210"));
scr->String_P(PSTR("sSdrilSE"));
uint8_t	uk = 0;
uint8_t	dat;
DDRF = 0;
PORTF = 255;
for (;;)
{
	((Lcd_hard *) scr)->Interrupt_static();
// 	scr->Hex((uint8_t)0, PINF);
	switch(uk)
	{
		case 0:
			scr->SetPosition2(0, 0);
			dat = ns_pins::transfer_data();
			scr->PutChar('0' + bit_is_byte(dat).bit7);
			break;
		case 1:
			scr->PutChar('0' + bit_is_byte(dat).bit6);
			break;
		case 2:
			scr->PutChar('0' + bit_is_byte(dat).bit5);
			break;
		case 3:
			scr->PutChar('0' + bit_is_byte(dat).bit4);
			break;
		case 4:
			scr->PutChar('0' + bit_is_byte(dat).bit3);
			break;
		case 5:
			scr->PutChar('0' + bit_is_byte(dat).bit2);
			break;
		case 6:
			scr->PutChar('0' + bit_is_byte(dat).bit1);
			break;
		case 7:
			scr->PutChar('0' + bit_is_byte(dat).bit0);
			break;
		//
		case 8:
			scr->PutChar('0' + ns_pins::transfer_sprocket());
			break;
		case 9:
		scr->PutChar('0' + ns_pins::transfer_strobe());
		break;
		case 10:
		scr->PutChar('0' + ns_pins::transfer_dataEnable());
		break;
		case 11:
		scr->PutChar('0' + ns_pins::transfer_readyBusy());
		break;
		case 12:
		scr->PutChar('0' + ns_pins::transfer_slewInc());
		break;
		case 13:
		scr->PutChar('0' + ns_pins::transfer_leftRight());
		break;
		case 14:
		scr->PutChar('0' + ns_pins::transfer_startStop());
		break;
		case 15:
		scr->PutChar('0' + ns_pins::transfer_eotOrRhu());
		break;

	}
	if (++uk >= 16)	uk = 0;
	
	scr->Hex(17, PINF);
	scr->Hex(20, dat);
}
*/

	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		Core::idleCycle();
    }
}

