/*
 * core_def.h
 *
 * Created: 03.04.2024 15:37:35
 *  Author: User
 */ 


#ifndef CORE_DEF_H_
#define CORE_DEF_H_

#include <avr/io.h>
#include <util/atomic.h>
#include "bits.h"

typedef			void(*TypeMetod)();
#define			callMetod(metod)	((TypeMetod)(metod))();

#define		selectByte(w, n)	((uint8_t *)&w)[n]

#define  CRITICAL_SECTION ATOMIC_BLOCK(ATOMIC_RESTORESTATE)

#define EIMSK_INT0		bit0
#define EIMSK_INT1		bit1
#define EIMSK_INT2		bit2
#define EIMSK_INT3		bit3
#define EIMSK_INT4		bit4
#define EIMSK_INT5		bit5
#define EIMSK_INT6		bit6
#define EIMSK_INT7		bit7

#endif /* CORE_DEF_H_ */