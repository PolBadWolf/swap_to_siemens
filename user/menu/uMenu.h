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



#define		_M_M1				0
#define		_M_SCREEN1			1			// начальный экран
#define		_M_COPY				2			// копирование программы с одного сектора в другой
#define		_M_CLEAR			3			// стирание сектора/блока
#define		_M_REQE_READ		4			// окончание чтени€ программы или пуск чтени€ части программы
#define		_M_READ_PART		5			// чтение части программы
#define		_M_VIEW_BLOCK		6			// чтение блока/сектора пам€ти
#define		_M_REQE_WRITE		7			// выбор части программы дл€ записи
#define		_M_WRITE_PART		8			// запись(отправка) части программы
//
#define		_M_PLUS5			9			// подт€жка входов вкл/выкл
#define		_M_EOT				10			// логический уровень выхода eot
#define		_M_PINS				11

#ifdef		__DEBUG_SIMULYATION
#define		_M_SIMUL			12			// симул€ци€ приема в сектор 3210 байт
#endif


//      0
void	m1_init();
void	m1_view();
void	m1_key2();
void	m1_key3();
void	m1_key1();
void	m1_key4();


//        1
void	screen1_init();
void	screen1_k1();
void	screen1_k2();
void	screen1_k3();
void	screen1_k4();
void	screen1_check();

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

// ======== read party ======
void	readParty_init();
void	readParty_view();
void	readParty_k4();

// ====== view block =======
void	viewBlock_init();
void	viewBlock_view();
void	viewBlock_k1();
void	viewBlock_k2();
void	viewBlock_k3();

// ==== reqe write ========
void	reqeWrite_begin();
void	reqeWrite_init();
void	reqeWrite_k2();
void	reqeWrite_k3();

// ==== write party =======
void	writeParty_init();
void	writeParty_view();
void	writeParty_k1();

// === plus5 ====
void	plus5_init();
void	plus5_k1();
void	plus5_k2();
void	plus5_k3();
void	plus5_k4();

// === eot ===
void	eot_init();
void	eot_k1();
void	eot_k2();
void	eot_k3();
void	eot_k4();

// === pins ===
void	pins_init();
void	pins_view();
void	pins_k1();


#endif // CONF_MENU
#endif //__UMENU_H__
