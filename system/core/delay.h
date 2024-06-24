/*
 * delay.h
 *
 * Created: 08.04.2024 15:56:48
 *  Author: User
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#include <avr/io.h>

#define __delay_us(__us) __builtin_avr_delay_cycles((F_CPU / 1000000L) * __us)
#define __delay_ms(__ms) __builtin_avr_delay_cycles(((uint32_t)F_CPU / (uint32_t)1000L) * __ms)
void	__delay_ms_irq(uint16_t	_ms);

void	delay_irq();

#endif /* DELAY_H_ */