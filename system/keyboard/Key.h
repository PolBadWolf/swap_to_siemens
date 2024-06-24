/* 
* Key.h
*
* Created: 15.04.2024 14:21:55
* Author: User
*/


#ifndef __KEY_H__
#define __KEY_H__

#include "core/core_def.h"
#include "core/Fifo.h"

class Key
{
//variables
public:
protected:
	uint8_t		stbKey;
	uint8_t		autoRepeat;
	static		Fifo<uint8_t>		*keyBuffer;
private:

//functions
public:
	void		autoRepeatOn();
	void		autoRepeatOff();
	uint8_t		inKey();
	uint8_t		readKey(uint8_t *key);
	uint8_t		getFill();
	void		resetBuff();
protected:
private:
// 	Key( const Key &c );
// 	Key& operator=( const Key &c );

}; //Key

#endif //__KEY_H__
