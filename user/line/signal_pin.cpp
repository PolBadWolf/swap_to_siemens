/* 
* signal_pin.cpp
*
* Created: 16.05.2024 13:12:56
* Author: User
*/


#include "signal_pin.h"
#include "core/bits.h"
#include <avr/eeprom.h>
#include "user/var.h"

// ***************** CONFIG PINS ******************
// #define		__MB_DEBUG__

#ifdef		__MB_DEBUG__
// ============== data ===============
#define		d_bit0_bit			bit0
#define		d_bit0_ddr			bit_is_byte(DDRC).d_bit0_bit
#define		d_bit0_out			bit_is_byte(PORTC).d_bit0_bit
#define		d_bit0_inp			bit_is_byte(PINC).d_bit0_bit

#define		d_bit1_bit			bit1
#define		d_bit1_ddr			bit_is_byte(DDRC).d_bit1_bit
#define		d_bit1_out			bit_is_byte(PORTC).d_bit1_bit
#define		d_bit1_inp			bit_is_byte(PINC).d_bit1_bit

#define		d_bit2_bit			bit2
#define		d_bit2_ddr			bit_is_byte(DDRC).d_bit2_bit
#define		d_bit2_out			bit_is_byte(PORTC).d_bit2_bit
#define		d_bit2_inp			bit_is_byte(PINC).d_bit2_bit

#define		d_bit3_bit			bit3
#define		d_bit3_ddr			bit_is_byte(DDRC).d_bit3_bit
#define		d_bit3_out			bit_is_byte(PORTC).d_bit3_bit
#define		d_bit3_inp			bit_is_byte(PINC).d_bit3_bit

#define		d_bit4_bit			bit4
#define		d_bit4_ddr			bit_is_byte(DDRC).d_bit4_bit
#define		d_bit4_out			bit_is_byte(PORTC).d_bit4_bit
#define		d_bit4_inp			bit_is_byte(PINC).d_bit4_bit

#define		d_bit5_bit			bit5
#define		d_bit5_ddr			bit_is_byte(DDRC).d_bit5_bit
#define		d_bit5_out			bit_is_byte(PORTC).d_bit5_bit
#define		d_bit5_inp			bit_is_byte(PINC).d_bit5_bit

#define		d_bit6_bit			bit6
#define		d_bit6_ddr			bit_is_byte(DDRC).d_bit6_bit
#define		d_bit6_out			bit_is_byte(PORTC).d_bit6_bit
#define		d_bit6_inp			bit_is_byte(PINC).d_bit6_bit

#define		d_bit7_bit			bit7
#define		d_bit7_ddr			bit_is_byte(DDRC).d_bit7_bit
#define		d_bit7_out			bit_is_byte(PORTC).d_bit7_bit
#define		d_bit7_inp			bit_is_byte(PINC).d_bit7_bit

// ============== spocket =====================
#define		sprocket_bit		bit5
#define		sprocket_ddr		bit_is_byte(DDRD).sprocket_bit
#define		sprocket_out		bit_is_byte(PORTD).sprocket_bit
#define		sprocket_inp		bit_is_byte(PIND).sprocket_bit

// ============== strobe ======================
#define		strobe_bit			bit3
#define		strobe_ddr			bit_is_byte(DDRD).strobe_bit
#define		strobe_out			bit_is_byte(PORTD).strobe_bit
#define		strobe_inp			bit_is_byte(PIND).strobe_bit

// ================= startStop ==================
#define		startStop_bit		bit7
#define		startStop_ddr		bit_is_byte(DDRD).startStop_bit
#define		startStop_out		bit_is_byte(PORTD).startStop_bit
#define		startStop_inp		bit_is_byte(PIND).startStop_bit

// ================= eotOrRhu ===================
#define		eotOrRhu_bit		bit6
#define		eotOrRhu_ddr		bit_is_byte(DDRD).eotOrRhu_bit
#define		eotOrRhu_out		bit_is_byte(PORTD).eotOrRhu_bit
#define		eotOrRhu_inp		bit_is_byte(PIND).eotOrRhu_bit
// -------------------------------------------------------------
#else
// ==================******************************============================= //
// ============== data ===============
#define		d_bit0_bit			bit0
#define		d_bit0_ddr			bit_is_byte(DDRF).d_bit0_bit
#define		d_bit0_out			bit_is_byte(PORTF).d_bit0_bit
#define		d_bit0_inp			bit_is_byte(PINF).d_bit0_bit

#define		d_bit1_bit			bit1
#define		d_bit1_ddr			bit_is_byte(DDRF).d_bit1_bit
#define		d_bit1_out			bit_is_byte(PORTF).d_bit1_bit
#define		d_bit1_inp			bit_is_byte(PINF).d_bit1_bit

#define		d_bit2_bit			bit2
#define		d_bit2_ddr			bit_is_byte(DDRF).d_bit2_bit
#define		d_bit2_out			bit_is_byte(PORTF).d_bit2_bit
#define		d_bit2_inp			bit_is_byte(PINF).d_bit2_bit

#define		d_bit3_bit			bit3
#define		d_bit3_ddr			bit_is_byte(DDRF).d_bit3_bit
#define		d_bit3_out			bit_is_byte(PORTF).d_bit3_bit
#define		d_bit3_inp			bit_is_byte(PINF).d_bit3_bit

#define		d_bit4_bit			bit4
#define		d_bit4_ddr			bit_is_byte(DDRF).d_bit4_bit
#define		d_bit4_out			bit_is_byte(PORTF).d_bit4_bit
#define		d_bit4_inp			bit_is_byte(PINF).d_bit4_bit

#define		d_bit5_bit			bit5
#define		d_bit5_ddr			bit_is_byte(DDRF).d_bit5_bit
#define		d_bit5_out			bit_is_byte(PORTF).d_bit5_bit
#define		d_bit5_inp			bit_is_byte(PINF).d_bit5_bit

#define		d_bit6_bit			bit6
#define		d_bit6_ddr			bit_is_byte(DDRF).d_bit6_bit
#define		d_bit6_out			bit_is_byte(PORTF).d_bit6_bit
#define		d_bit6_inp			bit_is_byte(PINF).d_bit6_bit

#define		d_bit7_bit			bit7
#define		d_bit7_ddr			bit_is_byte(DDRF).d_bit7_bit
#define		d_bit7_out			bit_is_byte(PORTF).d_bit7_bit
#define		d_bit7_inp			bit_is_byte(PINF).d_bit7_bit

// ============== spocket =====================
#define		sprocket_bit		bit2
#define		sprocket_ddr		bit_is_byte(DDRE).sprocket_bit
#define		sprocket_out		bit_is_byte(PORTE).sprocket_bit
#define		sprocket_inp		bit_is_byte(PINE).sprocket_bit

// ============== strobe ======================
#define		strobe_bit			bit3
#define		strobe_ddr			bit_is_byte(DDRE).strobe_bit
#define		strobe_out			bit_is_byte(PORTE).strobe_bit
#define		strobe_inp			bit_is_byte(PINE).strobe_bit

// ================= startStop ==================
#define		startStop_bit		bit6
#define		startStop_ddr		bit_is_byte(DDRE).startStop_bit
#define		startStop_out		bit_is_byte(PORTE).startStop_bit
#define		startStop_inp		bit_is_byte(PINE).startStop_bit

// ================= eotOrRhu ===================
#define		eotOrRhu_bit		bit7
#define		eotOrRhu_ddr		bit_is_byte(DDRE).eotOrRhu_bit
#define		eotOrRhu_out		bit_is_byte(PORTE).eotOrRhu_bit
#define		eotOrRhu_inp		bit_is_byte(PINE).eotOrRhu_bit
// -------------------------------------------------------------
#endif

// ********************************************************************
namespace	ns_pins
{
	// =================== data trand ==================
	void	transfer_data(uint8_t dat)
	{
		d_bit0_out = bit_is_byte(dat).bit0;
		d_bit1_out = bit_is_byte(dat).bit1;
		d_bit2_out = bit_is_byte(dat).bit2;
		d_bit3_out = bit_is_byte(dat).bit3;
		d_bit4_out = bit_is_byte(dat).bit4;
		d_bit5_out = bit_is_byte(dat).bit5;
		d_bit6_out = bit_is_byte(dat).bit6;
		d_bit7_out = bit_is_byte(dat).bit7;
	}
	uint8_t		transfer_data()
	{
		uint8_t dat = 0;
		bit_is_byte(dat).bit0 = d_bit0_inp;
		bit_is_byte(dat).bit1 = d_bit1_inp;
		bit_is_byte(dat).bit2 = d_bit2_inp;
		bit_is_byte(dat).bit3 = d_bit3_inp;
		bit_is_byte(dat).bit4 = d_bit4_inp;
		bit_is_byte(dat).bit5 = d_bit5_inp;
		bit_is_byte(dat).bit6 = d_bit6_inp;
		bit_is_byte(dat).bit7 = d_bit7_inp;
		return	dat;
	}
	void	init_dataInp()
	{
		uint8_t		r = eeprom_read_byte(&ns_var::flag_plus5);
		if (r == 0)
		{
			transfer_data(0);
		} 
		else
		{
			transfer_data(0xff);
		}
		d_bit0_ddr = 0;
		d_bit1_ddr = 0;
		d_bit2_ddr = 0;
		d_bit3_ddr = 0;
		d_bit4_ddr = 0;
		d_bit5_ddr = 0;
		d_bit6_ddr = 0;
		d_bit7_ddr = 0;
	}
	void	init_dataOut()
	{
		transfer_data(0xff);
		d_bit0_ddr = 1;
		d_bit1_ddr = 1;
		d_bit2_ddr = 1;
		d_bit3_ddr = 1;
		d_bit4_ddr = 1;
		d_bit5_ddr = 1;
		d_bit6_ddr = 1;
		d_bit7_ddr = 1;
	}

	// ============== spocket =====================
	void	init_sprocketInp()
	{
		sprocket_out = eeprom_read_byte(&ns_var::flag_plus5);
		sprocket_ddr = 0;
	}
	void	init_sprocketOut()
	{
		sprocket_out = 1;
		sprocket_ddr = 1;
	}
	uint8_t	transfer_sprocket()
	{
		return	sprocket_inp;
	}
	void	transfer_sprocket(uint8_t dat)
	{
		sprocket_out = dat;
	}

	// ============== strobe =====================
	void	init_strobeInp()
	{
		strobe_out	= eeprom_read_byte(&ns_var::flag_plus5);
		strobe_ddr	= 0;
	}
	void	init_strobeOut()
	{
		strobe_out	= 0;
		strobe_ddr	= 1;
	}
	uint8_t	transfer_strobe()
	{
		return	strobe_inp;
	}
	void	transfer_strobe(uint8_t dat)
	{
		strobe_out = dat;
	}

	// ================== startStop ==================
	void	init_startStopInp()
	{
		startStop_out = eeprom_read_byte(&ns_var::flag_plus5);
		startStop_ddr = 0;
	}
	void	init_startStopOut()
	{
		startStop_out = 0;
		startStop_ddr = 1;
	}
	uint8_t	transfer_startStop()
	{
		return	startStop_inp;
	}
	void	transfer_startStop(uint8_t dat)
	{
		startStop_out = dat;
	}
	
	// ================== eot or rhu ==================
	void	init_eotOrRhuInp()
	{
		eotOrRhu_out	= eeprom_read_byte(&ns_var::flag_plus5);
		eotOrRhu_ddr	= 0;
	}
	void	init_eotOrRhuOut()
	{
		eotOrRhu_out	= eeprom_read_byte(&ns_var::flag_eot);
		eotOrRhu_ddr	= 1;
	}
	uint8_t	transfer_eotOrRhu()
	{
		return	eotOrRhu_inp;
	}
	void	transfer_eotOrRhu(uint8_t dat)
	{
		eotOrRhu_out	= dat;
	}
	
}







