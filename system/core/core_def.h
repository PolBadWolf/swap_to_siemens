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

#endif /* CORE_DEF_H_ */