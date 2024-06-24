/* 
* Sd.h
*
* Created: 23.04.2024 13:19:28
* Author: User
*/


#ifndef __SD_H__
#define __SD_H__

#include "core/core_def.h"

class Sd
{
//variables
public:
protected:
private:

//functions
public:
	Sd();
	~Sd();
	// =================
	uint8_t		init();
	uint8_t		sendCommand(uint8_t	dt0, uint8_t dt1, uint8_t dt2, uint8_t dt3, uint8_t dt4, uint8_t dt5);
	uint8_t		write_Block(uint8_t	*bf, uint8_t dt1, uint8_t dt2, uint8_t dt3, uint8_t dt4);
	uint8_t		read_Block (uint8_t	*bf, uint8_t dt1, uint8_t dt2, uint8_t dt3, uint8_t dt4);
protected:
private:
	Sd( const Sd &c );
	Sd& operator=( const Sd &c );

}; //Sd

#endif //__SD_H__
