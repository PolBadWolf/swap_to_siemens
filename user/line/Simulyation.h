/* 
* Simulyation.h
*
* Created: 26.06.2024 13:17:10
* Author: User
*/


#ifndef __SIMULYATION_H__
#define __SIMULYATION_H__

#include "core/core.h"

namespace	ns_simul
{
	void	read_init();
	void	read_On();
	void	read_cycle();
}

// vars
namespace	ns_simul
{
	extern		const	uint16_t	flsh_avr_startAdr;
	extern		uint16_t	read_avr_adr;
	extern		uint16_t	read_lenght;
}


#endif //__SIMULYATION_H__
