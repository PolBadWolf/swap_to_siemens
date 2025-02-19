/* 
* uMenu.h
*
* Created: 23.04.2024 10:26:20
* Author: User
*/


#ifndef __UMENU_H__
#define __UMENU_H__

#include "config_sys.h"
#ifdef CONF_MENU

#include "user/var.h"

// ===============================
void	user_menu_init();
// ===============================



#define		_M_START			0
#define		_M_SCREEN1			1			// начальный экран
#define		_M_COPY				2			// копирование программы с одного сектора в другой
#define		_M_CLEAR			3			// стирание сектора/блока
#define		_M_REQE_READ		4			// окончание чтения программы или пуск чтения части программы
#define		_M_READ_PART		5			// чтение части программы
#define		_M_VIEW_BLOCK		6			// чтение блока/сектора памяти
#define		_M_REQE_WRITE		7			// выбор части программы для записи
#define		_M_WRITE_PART		8			// запись(отправка) части программы
//
#define		_M_LEFT_RIGHT_INV	9
#define		_M_PINS				10
#define		_M_WT_SCR1			11			// ожидание и переход на скрин1

#define		_M_SD_lenMinus		12			//	количество байт минус

#define		_M_SPR_lenDn		13			//	длительность сигнала SPR в 0 состояниии
#define		_M_SPR_lenUp		14			//	длительность сигнала SPR в 1 состояниии

#define		_M_Buse_lenPre		15			//	длительность сигнала BUSY в до
#define		_M_Buse_lenAft		16			//	длительность сигнала BUSY в после



void	menuUser_1Milisec();

//      0
void	start_init();
void	start_count();
void	start_key1();
void	start_key2();
void	start_key3();
void	start_key4();


//        1
void	screen1_init();
void	screen1_k1();
void	screen1_k2();
void	screen1_k3();
void	screen1_k4();
void	screen1_check();
void	screen1_view();

//	======= copy =======
void	copy_init();
void	copy_k2();
void	copy_k3();
void	copy_k1();
void	copy_k4();

// ======== clear =======
void	clear_init();
void	clear_no();
void	clear_yes();

// ======== read data ======
void	reqeRead_begin();
void	reqeRead_init();
void	reqeRead_k2();
void	reqeRead_k3();
void	reqeRead_check();

// ======== send party ======
void	readParty_init();
void	readParty_view();
void	readParty_k4();

// ====== view block =======
void	viewBlock_init();
void	viewBlock_view();
void	viewBlock_view_1();
void	viewBlock_view_2();
void	viewBlock_k1();
void	viewBlock_k2();
void	viewBlock_k3();
void	viewBlock_k4();
// ==== reqe send ========
void	reqeSend_begin();
void	reqeSend_init();
void	reqeSend_k2();
void	reqeSend_k3();
void	reqeSend_check();

// ==== send party =======
void	sendParty_init();
void	sendParty_view();
void	sendParty_k1();

// === pins ===
void	pins_init();
void	pins_view();
void	pins_k1();

void wt_scr1_go();
// =================
void	len_minus_init();
void	len_minus_k1();
void	len_minus_k2();
void	len_minus_k3();
void	len_minus_k4();

// =================
void	leftRight_init();
void	leftRight_k1();
void	leftRight_k2();
void	leftRight_k3();
void	leftRight_k4();
// =================
void	spr_lenDn_init();
void	spr_lenDn_view();
void	spr_lenDn_k1();
void	spr_lenDn_k2();
void	spr_lenDn_k3();
void	spr_lenDn_k4();

// =================
void	spr_lenUp_init();
void	spr_lenUp_view();
void	spr_lenUp_k1();
void	spr_lenUp_k2();
void	spr_lenUp_k3();
void	spr_lenUp_k4();

// =================
void	buse_lenPre_init();
void	buse_lenPre_view();
void	buse_lenPre_k2();
void	buse_lenPre_k3();
void	buse_lenPre_k4();
void	buse_lenPre_k1();

// =================
void	buse_lenAft_init();
void	buse_lenAft_view();
void	buse_lenAft_k2();
void	buse_lenAft_k3();
void	buse_lenAft_k4();
void	buse_lenAft_k1();

// =================


#endif // CONF_MENU
#endif //__UMENU_H__
