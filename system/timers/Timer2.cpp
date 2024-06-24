/* 
* Timer2.cpp
*
* Created: 10.04.2024 14:33:51
* Author: User
*/

#pragma GCC optimize("O0")

#include "Timer2.h"

namespace Core
{
	void	interrupt2();
}

uint16_t	Timer2::F_TIMER;

#ifdef __AVR_ATmega128__
// default constructor
Timer2::Timer2(uint16_t fclk)
{
	F_TIMER = fclk;
	uint8_t	tmp = 0;
	// set CTC mode
	bit_set_byte(&tmp, WGM21, 1);
	bit_set_byte(&tmp, WGM20, 0);
	bit_set_byte(&tmp, COM21, 0);
	bit_set_byte(&tmp, COM20, 0);
	// pre div
	uint16_t prDiv = (uint16_t)(F_CPU/(((uint32_t)fclk)*((uint32_t)256)));
	if (prDiv <= 1) {
		prDiv = 1;
		bit_set_byte(&tmp, CS20, 1);
		bit_set_byte(&tmp, CS21, 0);
		bit_set_byte(&tmp, CS22, 0);
	}	else
	if (prDiv <= 8) {
		prDiv = 8;
		bit_set_byte(&tmp, CS20, 0);
		bit_set_byte(&tmp, CS21, 1);
		bit_set_byte(&tmp, CS22, 0);
	}	else
	if (prDiv <= 64) {
		prDiv = 64;
		bit_set_byte(&tmp, CS20, 1);
		bit_set_byte(&tmp, CS21, 1);
		bit_set_byte(&tmp, CS22, 0);
	}	else
	if (prDiv <= 256) {
		prDiv = 256;
		bit_set_byte(&tmp, CS20, 0);
		bit_set_byte(&tmp, CS21, 0);
		bit_set_byte(&tmp, CS22, 1);
	}	else
	{
		prDiv = 1024;
		bit_set_byte(&tmp, CS20, 1);
		bit_set_byte(&tmp, CS21, 0);
		bit_set_byte(&tmp, CS22, 1);
	}
	// настройка частоты
	bit_set_byte(&tmp, FOC0, 0);
	TCCR2 = tmp;
	uint8_t div = (((unsigned long)F_CPU)/prDiv/fclk)-1;
	OCR2 = div;
	//
	TCNT2 = 0;
	bit_set_byte(&TIFR, OCF2, 0);
	bit_set_byte(&TIFR, TOV2, 0);
	bit_set_byte(&TIMSK, OCIE2, 1);
} //Timer2
#endif // __AVR_ATmega128__

#ifdef __AVR_ATmega16__
// default constructor
Timer2::Timer2(uint16_t fclk)
{
	F_TIMER = fclk;
	uint8_t	tmp = 0;
	// set CTC mode
	bit_set_byte(&tmp, WGM21, 1);
	bit_set_byte(&tmp, WGM20, 0);
	bit_set_byte(&tmp, COM21, 0);
	bit_set_byte(&tmp, COM20, 0);
	// pre div
	uint16_t prDiv = (uint16_t)(F_CPU/(((uint32_t)fclk)*((uint32_t)256)));
	if (prDiv <= 1) {
		prDiv = 1;
		bit_set_byte(&tmp, CS20, 1);
		bit_set_byte(&tmp, CS21, 0);
		bit_set_byte(&tmp, CS22, 0);
	}	else
	if (prDiv <= 8) {
		prDiv = 8;
		bit_set_byte(&tmp, CS20, 0);
		bit_set_byte(&tmp, CS21, 1);
		bit_set_byte(&tmp, CS22, 0);
	}	else
	if (prDiv <= 32) {
		prDiv = 32;
		bit_set_byte(&tmp, CS20, 1);
		bit_set_byte(&tmp, CS21, 1);
		bit_set_byte(&tmp, CS22, 0);
	}	else
	if (prDiv <= 64) {
		prDiv = 64;
		bit_set_byte(&tmp, CS20, 0);
		bit_set_byte(&tmp, CS21, 0);
		bit_set_byte(&tmp, CS22, 1);
	}	else
	if (prDiv <= 128) {
		prDiv = 128;
		bit_set_byte(&tmp, CS20, 1);
		bit_set_byte(&tmp, CS21, 0);
		bit_set_byte(&tmp, CS22, 1);
	}	else
	if (prDiv <= 256) {
		prDiv = 256;
		bit_set_byte(&tmp, CS20, 0);
		bit_set_byte(&tmp, CS21, 1);
		bit_set_byte(&tmp, CS22, 1);
	}	else
	{
		prDiv = 1024;
		bit_set_byte(&tmp, CS20, 1);
		bit_set_byte(&tmp, CS21, 1);
		bit_set_byte(&tmp, CS22, 1);
	}
	// настройка частоты
	bit_set_byte(&tmp, FOC0, 0);
	TCCR2 = tmp;
	uint8_t div = (((unsigned long)F_CPU)/prDiv/fclk)-1;
	OCR2 = div;
	//
	TCNT2 = 0;
	bit_set_byte(&TIFR, OCF2, 0);
	bit_set_byte(&TIFR, TOV2, 0);
	bit_set_byte(&TIMSK, OCIE2, 1);
} //Timer2
#endif // __AVR_ATmega16__

// default destructor
Timer2::~Timer2()
{
} //~Timer2

ISR(TIMER2_COMP_vect)
{
	// 		ns_device::Timer_Usr();
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

	bit_set_byte(&TIMSK, OCIE2, 0);
	Core::interrupt2();
	bit_set_byte(&TIMSK, OCIE2, 1);

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
