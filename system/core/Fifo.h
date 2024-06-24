/* 
* Fifo.h
*
* Created: 08.04.2024 9:51:13
* Author: User
*/


#ifndef __FIFO_H__
#define __FIFO_H__

#include "core/core_def.h"

template <class Unit>
class Fifo
{
//variables
public:
	Unit		*units;
protected:
	uint8_t		size;
	uint8_t		head;
	uint8_t		tail;
private:

//functions
public:
	Fifo(uint8_t size) {
		this->size	= size;
		this->units	= new Unit[size];
		this->head	= 0;
		this->tail	= 0;
	}
	uint8_t		push(Unit		unit) {
		uint8_t	next = normUpLite(head + 1);
		if (next == tail)	return	1;
		CRITICAL_SECTION {
			units[head] = unit;
			head = next;
		}
		return	0;
	}
	uint8_t		pushFast(Unit	unit) {
		uint8_t	next = normUpLite(head + 1);
		if (next == tail)	return	1;
		units[head] = unit;
		head = next;
		return	0;
	}
	uint8_t		pop(Unit		*unit) {
		if (tail == head)	return	1;
		uint8_t	next = normUpLite(tail + 1);
		CRITICAL_SECTION {
			*unit = units[tail];
			tail = next;
		}
		return	0;
	}
	uint8_t		popFast(Unit	*unit) {
		if (tail == head)	return	1;
		*unit = units[tail];
		tail = normUpLite(tail + 1);
		return	0;
	}
	uint8_t		getFill() {
		if (head < tail)	return	head + (size - tail);
		return	head - tail;
	}
	//
	void		resetBuff()
	{
		uint8_t k;
		while(pop(&k));
	}
protected:
	uint8_t		normUpLite(uint8_t	addr) {
		if (addr >= size)	return	0;
		return	addr;
	}
private:
// 	Fifo( const Fifo &c );
// 	Fifo& operator=( const Fifo &c );

}; //Fifo

#endif //__FIFO_H__
