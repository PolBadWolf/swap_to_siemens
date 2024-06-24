/* 
* RS232.cpp
*
* Created: 12.04.2024 9:14:43
* Author: User
*/


#include "config_sys.h"


#include "RS232.h"
#include "core/bits.h"

// default constructor
RS232::RS232()
{
	obj = this;
} //RS232

// default destructor
RS232::~RS232()
{
} //~RS232

RS232		*RS232::obj;

// ======================================================================
#ifdef __AVR_ATmega16__
Serial*		RS232::init(rsBaud baud, rsParity parity, rsBit bit, uint8_t sizeBuffInp, uint8_t sizeBuffOut)
{
	RS232	*rs = new RS232();
	// create buff
	rs->buffInp = new Fifo<uint8_t>(sizeBuffInp);
	rs->BuffOut = new Fifo<uint8_t>(sizeBuffOut);
	// set speed
	uint16_t	ubrr = (F_CPU / 16 / baud) - 1;
	UBRRH =	word_to_byte(ubrr).High;
	UBRRL = word_to_byte(ubrr).Low;
	//
	UCSRA = 0;
	uint8_t	tmp;
	// прерывания
	tmp = 0;
	bit_set_byte(&tmp, RXEN,  1);		// разрешение на прием
	bit_set_byte(&tmp, TXEN,  1);		// разрешение на передачу
	bit_set_byte(&tmp, RXCIE, 1);		// прерывание на прием
	UCSRB = tmp;
	
	tmp = 0;
	// четность
	switch(parity)
	{
		case DISABLE:
			bit_set_byte(&tmp, UPM1, 0);
			bit_set_byte(&tmp, UPM0, 0);
			break;
		case EVEN:
			bit_set_byte(&tmp, UPM1, 1);
			bit_set_byte(&tmp, UPM0, 0);
			break;
		case ODD:
			bit_set_byte(&tmp, UPM1, 1);
			bit_set_byte(&tmp, UPM0, 1);
			break;
		default:
			break;
	}
	// 2 стоп бита
	bit_set_byte(&tmp, USBS, 1);
	// количество бит данных
	switch(bit)
	{
		case bit5:
			bit_set_byte(&tmp, UCSZ0, 0);
			bit_set_byte(&tmp, UCSZ1, 0);
			obj->mask = 0x1f;
			break;
		case bit6:
			bit_set_byte(&tmp, UCSZ0, 1);
			bit_set_byte(&tmp, UCSZ1, 0);
			obj->mask = 0x3f;
			break;
		case bit7:
			bit_set_byte(&tmp, UCSZ0, 0);
			bit_set_byte(&tmp, UCSZ1, 1);
			obj->mask = 0x7f;
			break;
		case bit8:
			bit_set_byte(&tmp, UCSZ0, 1);
			bit_set_byte(&tmp, UCSZ1, 1);
			obj->mask = 0xff;
			break;
		default:
			bit_set_byte(&tmp, UCSZ0, 1);
			bit_set_byte(&tmp, UCSZ1, 1);
			obj->mask = 0xff;
	}
	// --------
	UCSRC = tmp;
	
	return	rs;
}
// отправка байта
bool	RS232::sendByte(uint8_t dat)
{
	bool r = BuffOut->push(dat);
	bit_set_byte(&UCSRB, UDRIE, 1);
	return	r;
}

ISR(USART_UDRE_vect)
{
	RS232::obj->_interruptSend();
}

void	RS232::_interruptSend()
{
	uint8_t		dat;
	if (BuffOut->pop(&dat))
	{
		bit_set_byte(&UCSRB, UDRIE, 0);
		return;
	}
}

ISR(USART_RXC_vect)
{
	RS232::obj->_interruptAccept();
}

void	RS232::_interruptAccept()
{
	volatile uint8_t	dat = UDR & mask;
	buffInp->push(dat);
}

bool	RS232::readByte(uint8_t *dat)
{
	return	buffInp->pop(dat);
}


#endif // __AVR_ATmega16__
// ----------------------------------------------------------------------

// ======================================================================
#ifdef __AVR_ATmega128__
Serial*		RS232::init(rsBaud baud, rsParity parity, rsBit bit, uint8_t sizeBuffInp, uint8_t sizeBuffOut)
{
	RS232	*rs = new RS232();
	// create buff
	rs->buffInp = new Fifo<uint8_t>(sizeBuffInp);
	rs->BuffOut = new Fifo<uint8_t>(sizeBuffOut);
	// set speed
	uint16_t	ubrr = (F_CPU / 16 / baud) - 1;
	UBRR0H =	word_to_byte(ubrr).High;
	UBRR0L = word_to_byte(ubrr).Low;
	//
	UCSR0A = 0;
	uint8_t	tmp;
	// прерывания
	tmp = 0;
	bit_set_byte(&tmp, RXEN0,  1);		// разрешение на прием
	bit_set_byte(&tmp, TXEN0,  1);		// разрешение на передачу
	bit_set_byte(&tmp, RXCIE0, 1);		// прерывание на прием
	UCSR0B = tmp;
	
	tmp = 0;
	// четность
	switch(parity)
	{
		case DISABLE:
		bit_set_byte(&tmp, UPM01, 0);
		bit_set_byte(&tmp, UPM00, 0);
		break;
		case EVEN:
		bit_set_byte(&tmp, UPM01, 1);
		bit_set_byte(&tmp, UPM00, 0);
		break;
		case ODD:
		bit_set_byte(&tmp, UPM01, 1);
		bit_set_byte(&tmp, UPM00, 1);
		break;
		default:
		break;
	}
	// 2 стоп бита
	bit_set_byte(&tmp, USBS0, 1);
	// количество бит данных
	switch(bit)
	{
		case bit5:
		bit_set_byte(&tmp, UCSZ00, 0);
		bit_set_byte(&tmp, UCSZ01, 0);
		obj->mask = 0x1f;
		break;
		case bit6:
		bit_set_byte(&tmp, UCSZ00, 1);
		bit_set_byte(&tmp, UCSZ01, 0);
		obj->mask = 0x3f;
		break;
		case bit7:
		bit_set_byte(&tmp, UCSZ00, 0);
		bit_set_byte(&tmp, UCSZ01, 1);
		obj->mask = 0x7f;
		break;
		case bit8:
		bit_set_byte(&tmp, UCSZ00, 1);
		bit_set_byte(&tmp, UCSZ01, 1);
		obj->mask = 0xff;
		break;
		default:
		bit_set_byte(&tmp, UCSZ00, 1);
		bit_set_byte(&tmp, UCSZ01, 1);
		obj->mask = 0xff;
	}
	// --------
	UCSR0C = tmp;
	
	return	rs;
}

// отправка байта
bool	RS232::sendByte(uint8_t dat)
{
	bool r = BuffOut->push(dat & mask);
	bit_set_byte(&UCSR0B, UDRIE0, 1);
	return	r;
}

ISR(USART0_UDRE_vect)
{
	RS232::obj->_interruptSend();
}

void	RS232::_interruptSend()
{
	uint8_t		dat;
	if (BuffOut->pop(&dat))
	{
		bit_set_byte(&UCSR0B, UDRIE0, 0);
		return;
	}
	UDR0 = dat;
}

ISR(USART0_RX_vect)
{
	RS232::obj->_interruptAccept();
}

void	RS232::_interruptAccept()
{
	volatile uint8_t	dat = UDR0 & mask;
	BuffOut->push(dat);
}

bool	RS232::readByte(uint8_t *dat)
{
	return	buffInp->pop(dat);
}


#endif // __AVR_ATmega128__
// ----------------------------------------------------------------------


