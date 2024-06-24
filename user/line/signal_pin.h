/* 
* signal_pin.h
*
* Created: 16.05.2024 13:12:57
* Author: User
*/


#ifndef __SIGNAL_PIN_H__
#define __SIGNAL_PIN_H__

#include "core/core.h"


namespace	ns_pins
{
	// =================== data trand ==================
	void	init_dataInp();
	void	init_dataOut();
	void	transfer_data(uint8_t dat);
	uint8_t	transfer_data();
	
	// ============== spocket =====================
	void	init_sprocketInp();
	void	init_sprocketOut();
	uint8_t	transfer_sprocket();
	void	transfer_sprocket(uint8_t dat);

	// ============== strobe =====================
	void	init_strobeInp();
	void	init_strobeOut();
	uint8_t	transfer_strobe();
	void	transfer_strobe(uint8_t dat);
	
	// ===================== dataEnable ==================
	void	init_dataEnableInp();
	void	init_dataEnableOut();
	uint8_t	transfer_dataEnable();
	void	transfer_dataEnable(uint8_t dat);

	// ====================== readyBusy =======================
	void	init_readyBusyInp();
	void	init_readyBusyOut();
	uint8_t	transfer_readyBusy();
	void	transfer_readyBusy(uint8_t dat);

	// ============ slewInc ======================
	void	init_slewIncInp();
	void	init_slewIncOut();
	uint8_t	transfer_slewInc();
	void	transfer_slewInc(uint8_t dat);

	// ============ leftRight ======================
	void	init_leftRightInp();
	void	init_leftRightOut();
	uint8_t	transfer_leftRight();
	void	transfer_leftRight(uint8_t dat);
	
	// ================== startStop ==================
	void	init_startStopInp();
	void	init_startStopOut();
	uint8_t	transfer_startStop();
	void	transfer_startStop(uint8_t dat);

	// ================== eot or rhu ==================
	void	init_eotOrRhuInp();
	void	init_eotOrRhuOut();
	uint8_t	transfer_eotOrRhu();
	void	transfer_eotOrRhu(uint8_t dat);
	
}


#endif //__SIGNAL_PIN_H__
