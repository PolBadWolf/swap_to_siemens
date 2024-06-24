/* 
* menu.cpp
*
* Created: 15.04.2024 9:44:25
* Author: User
*/


#include "config_sys.h"

#ifdef CONF_MENU

#include "menu.h"
// -------------- include ----------------
#include <avr/pgmspace.h>
#include "core/core_def.h"
#include "core/core.h"
#include "user/system/menuUser.h"
#include "user/system/menuUser.h"
#include "user/menu/uMenu.h"

#include "user/line/signal_pin.h"

// --------------- define ----------------
typedef		void(*TypeFunctionMenu)();
// ---------------- vars -----------------
namespace ns_menu
{
	uint8_t		keyCode;
	uint16_t	timeout;
	uint8_t		mode;
	extern		const	TypeFunctionMenu	massFunct[][8];
}
// -------------- functions --------------
namespace ns_menu
{
	void	functMenu_aft(uint8_t mode, uint8_t stat);
	void	functMenu_pre(uint8_t mode, uint8_t stat);
}
// ------------ code ---------------------

void	ns_menu::init()
{
	user_menu_init();
	timeout = 0;
}

void	ns_menu::mainCycle()
{
	uint8_t		keyStat = key->readKey(&keyCode);
	uint16_t	to;
	CRITICAL_SECTION
	{
		to = timeout;
	}
	if (to == 1)
	{
		CRITICAL_SECTION
		{
			timeout = 0;
		}
		functMenu_aft(mode, MENU_TIMEOUT);
	}
	else
	{
		functMenu_aft(mode, keyStat);
	}
	/*scr->Hex(16, PINF);
	uint8_t pin = 0;
	bit_is_byte(pin).bit7 = ns_pins::transfer_sprocket();
	bit_is_byte(pin).bit6 = ns_pins::transfer_strobe();
	bit_is_byte(pin).bit5 = ns_pins::transfer_dataEnable();
	bit_is_byte(pin).bit4 = ns_pins::transfer_readyBusy();
	bit_is_byte(pin).bit3 = ns_pins::transfer_slewInc();
	bit_is_byte(pin).bit2 = ns_pins::transfer_leftRight();
	bit_is_byte(pin).bit1 = ns_pins::transfer_startStop();
	bit_is_byte(pin).bit0 = ns_pins::transfer_eotOrRhu();
	scr->Hex(18, pin);
	scr->PutChar(20, '0' + bit_is_byte(pin).bit1);
	scr->PutChar(21, '0' + bit_is_byte(pin).bit0);
	*/
}

void	ns_menu::functMenu_aft(uint8_t mode, uint8_t stat)
{
	((TypeFunctionMenu) pgm_read_ptr(&massFunct[mode][stat]) )();
	if	(stat == MENU_SETMODE)	ns_menu::mode = mode;
}

void	ns_menu::functMenu_pre(uint8_t mode, uint8_t stat)
{
	if	(stat == MENU_SETMODE)	ns_menu::mode = mode;
	((TypeFunctionMenu) pgm_read_ptr(&massFunct[mode][stat]) )();
}

void	ns_menu::interrupt()
{
	CRITICAL_SECTION
	{
		if (timeout != 0)	timeout--;
	}
}

const	TypeFunctionMenu	ns_menu::massFunct[][8] PROGMEM =
{
#include "user/system/menuUserTab.h"
};

void	dump()
{
	
}

#endif // CONF_MENU



