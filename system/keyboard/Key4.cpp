/* 
* Key4.cpp
*
* Created: 15.04.2024 11:00:13
* Author: User
*/

#pragma GCC optimize("O0")

#include "config_sys.h"

#ifdef CONF_KEY4

#include "Key4.h"
#include "core/Fifo.h"

// ==============================================
#ifdef __AVR_ATmega128__
#define		KEY_DDR		(*((BYTE_KEY_DATA *) &DDRG)).data
#define		KEY_INP		(*((BYTE_KEY_DATA *) &PING)).data
#define		KEY_OUT		(*((BYTE_KEY_DATA *) &PORTG)).data
#define		TONE_DDR	DDRG
#define		TONE_OUT	PORTG
#define		TONE_BIT	4

typedef struct
{
	unsigned data:4;
	unsigned free:4;
} BYTE_KEY_DATA;
#endif // __AVR_ATmega128__

#ifdef __AVR_ATmega16__
#define		KEY_DDR		(*((BYTE_KEY_DATA *) &DDRD)).data
#define		KEY_INP		(*((BYTE_KEY_DATA *) &PIND)).data
#define		KEY_OUT		(*((volatile BYTE_KEY_DATA *) &PORTD)).data
#define		TONE_DDR	DDRC
#define		TONE_OUT	PORTC
#define		TONE_BIT	0

typedef struct
{
	unsigned free:4;
	unsigned data:4;
} BYTE_KEY_DATA;
#endif // __AVR_ATmega16__
// ----------------------------------------------

// ================ Tiks for interrupt 1 kHz ================
#define TIME_PUSH 50
#define TIME_PUSH_AUTO 50
#define TIME_AUTO 2000

#define TIME_TONE	80

#define KEY_BUFFER_SIZE	8

Key4	*Key4::obj	= 0;

// default destructor
Key4::~Key4()
{
} //~Key4

Key4*		Key4::init()
{
	if (obj == 0)
	{
		obj = new Key4;
	}
	return	obj;
}

// default constructor
Key4::Key4()
{
	// ============= настройка порта =================
	bit_set_byte(&TONE_OUT, TONE_BIT, 1);
	bit_set_byte(&TONE_DDR, TONE_BIT, 1);
	//
	KEY_OUT = 0x0f;
	KEY_DDR = 0x00;
	//
	oldKey				= ~KEY_INP;
	oldKey_count		= 0;
	//
	keyBuffer = new Fifo<uint8_t>(KEY_BUFFER_SIZE);
} //Key4

void	Key4::interrupt()
{
	obj->irq();
}

void	Key4::irq()
{
	// tone
	if (countTone > 0)
	{
		countTone--;
		bit_xor_byte(&TONE_OUT, TONE_BIT);
	}
	else	bit_set_byte(&TONE_OUT, TONE_BIT, 1);
	
	// stab
	uint8_t	curKey = KEY_INP;
	if (oldKey != curKey)
	{
		oldKey = curKey;
		oldKey_count = 0;
		
		return;
	}
	//
	oldKey_count++;
	uint8_t		key_send = 0;
	//
	if (autoRepeat)
	{
		if (oldKey_count >= TIME_AUTO)
		{
			bit_is_byte(key_send).bit0 = bit_is_byte(oldKey).bit3;
			bit_is_byte(key_send).bit1 = bit_is_byte(oldKey).bit2;
			bit_is_byte(key_send).bit2 = bit_is_byte(oldKey).bit1;
			bit_is_byte(key_send).bit3 = bit_is_byte(oldKey).bit0;
			keyBuffer->push(key_send ^ 0x0f);
			oldKey_count = TIME_AUTO - TIME_PUSH_AUTO;
			return;
		}
	}
	if (oldKey_count == TIME_PUSH)
	{
		stbKey = oldKey;
		bit_is_byte(key_send).bit0 = bit_is_byte(oldKey).bit3;
		bit_is_byte(key_send).bit1 = bit_is_byte(oldKey).bit2;
		bit_is_byte(key_send).bit2 = bit_is_byte(oldKey).bit1;
		bit_is_byte(key_send).bit3 = bit_is_byte(oldKey).bit0;
		keyBuffer->push(key_send ^ 0x0f);
	}
}

#endif // CONF_KEY4
