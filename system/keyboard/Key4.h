/* 
* Key4.h
*
* Created: 15.04.2024 11:00:13
* Author: User
*/


#ifndef __KEY4_H__
#define __KEY4_H__

#include "core/core_def.h"
#include "Key.h"

class Key4	: public Key
{
//variables
public:
	static Key4	*obj;
protected:
private:
	uint8_t		oldKey;			//
	uint16_t	oldKey_count;	//
	uint8_t		countTone;		//

//functions
public:
	static		Key4*	init();
	static void	interrupt();
	//--
	void		autoRepeatOn();
	void		autoRepeatOff();
 	uint8_t		inKey();
	uint8_t		readKey(uint8_t *key);
	uint8_t		getFill();
	void		resetBuff();
protected:
private:
	Key4( const Key4 &c );
	Key4& operator=( const Key4 &c );
	Key4();
	~Key4();
	void		irq();

}; //Key4

#endif //__KEY4_H__
