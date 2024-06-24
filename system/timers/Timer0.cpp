/* 
* Timer0.cpp
*
* Created: 04.04.2024 8:07:17
* Author: User
*/

#pragma GCC optimize("O0")

#include "Timer0.h"
#include <avr/pgmspace.h>

namespace	Core {
	void	interrupt0();
}

uint16_t	Timer0::F_TIMER;


	// ===================== init for mega 128 =====================
#ifdef __AVR_ATmega128__
// default constructor
Timer0::Timer0(uint16_t	fclk)
{
	F_TIMER = fclk;
	uint8_t		tmp_TCCR0;

	// set CTC mode
	tmp_TCCR0 = 0;
	bit_set_byte(&tmp_TCCR0, WGM01, 1);
	bit_set_byte(&tmp_TCCR0, WGM00, 0);
	bit_set_byte(&tmp_TCCR0, COM01, 0);
	bit_set_byte(&tmp_TCCR0, COM00, 0);

	// предварительный расчет предделителя
	volatile uint16_t prDiv = (uint16_t)( F_CPU / ( (uint32_t)F_TIMER *256 * 1) );
	// нормализация и установка предделителя
	if (prDiv <= 1) {
		prDiv = 1;
		bit_set_byte(&tmp_TCCR0, CS02, 0);
		bit_set_byte(&tmp_TCCR0, CS01, 0);
		bit_set_byte(&tmp_TCCR0, CS00, 1);
	}
	else if (prDiv <= 8) {
		prDiv = 8;
		bit_set_byte(&tmp_TCCR0, CS02, 0);
		bit_set_byte(&tmp_TCCR0, CS01, 1);
		bit_set_byte(&tmp_TCCR0, CS00, 0);
	}
	else if (prDiv <= 32) {
		prDiv = 32;
		bit_set_byte(&tmp_TCCR0, CS02, 0);
		bit_set_byte(&tmp_TCCR0, CS01, 1);
		bit_set_byte(&tmp_TCCR0, CS00, 1);
	}
	else if (prDiv <= 64) {
		prDiv = 64;
		bit_set_byte(&tmp_TCCR0, CS02, 1);
		bit_set_byte(&tmp_TCCR0, CS01, 0);
		bit_set_byte(&tmp_TCCR0, CS00, 0);
	}
	else if (prDiv <= 128) {
		prDiv = 128;
		bit_set_byte(&tmp_TCCR0, CS02, 1);
		bit_set_byte(&tmp_TCCR0, CS01, 0);
		bit_set_byte(&tmp_TCCR0, CS00, 1);
	}
	else if (prDiv <= 256) {
		prDiv = 256;
		bit_set_byte(&tmp_TCCR0, CS02, 1);
		bit_set_byte(&tmp_TCCR0, CS01, 1);
		bit_set_byte(&tmp_TCCR0, CS00, 0);
	}
	else
	{
		prDiv = 1024;
		bit_set_byte(&tmp_TCCR0, CS02, 1);
		bit_set_byte(&tmp_TCCR0, CS01, 1);
		bit_set_byte(&tmp_TCCR0, CS00, 1);
	}
	// режим работы и разрешение
	bit_set_byte(&tmp_TCCR0, FOC0, 1);
	TCCR0 = tmp_TCCR0;
	
	uint8_t div = (uint8_t)( ((uint32_t)F_CPU) / ( ((uint32_t)prDiv) * ((uint32_t)F_TIMER) * ((uint32_t)1) ) ) - 1;
	OCR0 = div;
	
	TIFR &= ~( (1 << OCF0) | (1 << TOV0) );
	
	bit_set_byte(&TIMSK, OCIE0, 1);

} //Timer0
#endif // __AVR_ATmega128__
	// ===================== init for mega 16  =====================
#ifdef __AVR_ATmega16__
// default constructor
Timer0::Timer0(uint16_t	fclk)
{
	F_TIMER = fclk;
	uint8_t		tmp_TCCR0;

	// set CTC mode
	tmp_TCCR0 = 0;
	bit_set_byte(&tmp_TCCR0, WGM01, 1);
	bit_set_byte(&tmp_TCCR0, WGM00, 0);
	bit_set_byte(&tmp_TCCR0, COM01, 0);
	bit_set_byte(&tmp_TCCR0, COM00, 0);

	// предварительный расчет предделителя
	volatile uint16_t prDiv = (uint16_t)( F_CPU / ( (uint32_t)F_TIMER * 256 * 2) );
	// нормализация и установка предделителя
	if (prDiv <= 1) {
		prDiv = 1;
		bit_set_byte(&tmp_TCCR0, CS02, 0);
		bit_set_byte(&tmp_TCCR0, CS01, 0);
		bit_set_byte(&tmp_TCCR0, CS00, 1);
	}
	else if (prDiv <= 8) {
		prDiv = 8;
		bit_set_byte(&tmp_TCCR0, CS02, 0);
		bit_set_byte(&tmp_TCCR0, CS01, 1);
		bit_set_byte(&tmp_TCCR0, CS00, 0);
	}
	else if (prDiv <= 64) {
		prDiv = 64;
		bit_set_byte(&tmp_TCCR0, CS02, 0);
		bit_set_byte(&tmp_TCCR0, CS01, 1);
		bit_set_byte(&tmp_TCCR0, CS00, 1);
	}
	else if (prDiv <= 256) {
		prDiv = 256;
		bit_set_byte(&tmp_TCCR0, CS02, 1);
		bit_set_byte(&tmp_TCCR0, CS01, 0);
		bit_set_byte(&tmp_TCCR0, CS00, 0);
	}
	else
	{
		prDiv = 1024;
		bit_set_byte(&tmp_TCCR0, CS02, 1);
		bit_set_byte(&tmp_TCCR0, CS01, 0);
		bit_set_byte(&tmp_TCCR0, CS00, 1);
	}
	// режим работы и разрешение
	bit_set_byte(&tmp_TCCR0, FOC0, 1);
	TCCR0 = tmp_TCCR0;
	
	uint8_t div = (uint8_t)( ((uint32_t)F_CPU) / ( ((uint32_t)prDiv) * ((uint32_t)F_TIMER) * ((uint32_t)1) ) ) - 1;
	OCR0 = div;
	
	TIFR &= ~( (1 << OCF0) | (1 << TOV0) );
	
	bit_set_byte(&TIMSK, OCIE0, 1);

} //Timer0
#endif // __AVR_ATmega16__


// default destructor
Timer0::~Timer0()
{
} //~Timer0


ISR (TIMER0_COMP_vect) {
	__asm__ __volatile__ (
	"push	r31				\n\t"
	"in		r31, __SREG__	\n\t"
	"push	r31				\n\t"
	"push	r0				\n\t"
	"push	r1				\n\t"
	"push	r2				\n\t"
	"push	r3				\n\t"
	"push	r4				\n\t"
	"push	r5				\n\t"
	"push	r6				\n\t"
	"push	r7				\n\t"
	"push	r8				\n\t"
	"push	r9				\n\t"
	"push	r10				\n\t"
	"push	r11				\n\t"
	"push	r12				\n\t"
	"push	r13				\n\t"
	"push	r14				\n\t"
	"push	r15				\n\t"
	"push	r16				\n\t"
	"push	r17				\n\t"
	"push	r18				\n\t"
	"push	r19				\n\t"
	"push	r20				\n\t"
	"push	r21				\n\t"
	"push	r22				\n\t"
	"push	r23				\n\t"
	"push	r24				\n\t"
	"push	r25				\n\t"
	"push	r26				\n\t"
	"push	r27				\n\t"
	"push	r28				\n\t"
	"push	r29				\n\t"
	"push	r30				\n\t"
	::
	);

	Core::interrupt0();
	
	cli();
	__asm__ __volatile__ (
	"pop	r30				\n\t"
	"pop	r29				\n\t"
	"pop	r28				\n\t"
	"pop	r27				\n\t"
	"pop	r26				\n\t"
	"pop	r25				\n\t"
	"pop	r24				\n\t"
	"pop	r23				\n\t"
	"pop	r22				\n\t"
	"pop	r21				\n\t"
	"pop	r20				\n\t"
	"pop	r19				\n\t"
	"pop	r18				\n\t"
	"pop	r17				\n\t"
	"pop	r16				\n\t"
	"pop	r15				\n\t"
	"pop	r14				\n\t"
	"pop	r13				\n\t"
	"pop	r12				\n\t"
	"pop	r11				\n\t"
	"pop	r10				\n\t"
	"pop	r9				\n\t"
	"pop	r8				\n\t"
	"pop	r7				\n\t"
	"pop	r6				\n\t"
	"pop	r5				\n\t"
	"pop	r4				\n\t"
	"pop	r3				\n\t"
	"pop	r2				\n\t"
	"pop	r1				\n\t"
	"pop	r0				\n\t"
	"pop	r31				\n\t"
	"out	__SREG__, r31	\n\t"
	"pop	r31				\n\t"
	);
}
