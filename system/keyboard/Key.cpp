/* 
* Key.cpp
*
* Created: 15.04.2024 14:21:55
* Author: User
*/


#include "Key.h"
#include "Key4.h"

Fifo<uint8_t>	*Key::keyBuffer;

// default constructor
// Key::Key()
// {
// } //Key
// 
// default destructor
// Key::~Key()
// {
// } //~Key

void	Key::autoRepeatOn()
{
	autoRepeat = 1;
}

void	Key::autoRepeatOff()
{
	autoRepeat = 0;
}

uint8_t	Key::inKey()
{
	return	stbKey;
}

uint8_t	Key::readKey(uint8_t	*key)
{
	uint8_t	stat = keyBuffer->pop(key);
	if (stat != 0)	return	0;
	if (*key == 1)	return	1;
	if (*key == 2)	return	2;
	if (*key == 4)	return	3;
	if (*key == 8)	return	4;
	return	5;
}

uint8_t	Key::getFill()
{
	return	keyBuffer->getFill();
}

void	Key::resetBuff()
{
	keyBuffer->resetBuff();
}
