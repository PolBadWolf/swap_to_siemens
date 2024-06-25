/* 
* var.h
*
* Created: 14.05.2024 9:37:04
* Author: User
*/


#ifndef __VAR_H__
#define __VAR_H__

#include <avr/eeprom.h>

// #define		__DEBUG_SIMULYATION

#define LIST_MOD_view	0
#define LIST_MOD_save	1
#define LIST_MOD_clr	2
#define LIST_MOD_copy	3
#define LIST_MOD_load	4
#define LIST_MOD_plus5	5
#define LIST_MOD_eot	6
#define LIST_MOD_pins	7

#define LIST_MOD__MAX	8

#define		OFFSET_WRITE	0x40


namespace	ns_var
{
	// === структура относительного адреса и длины
	struct StrucAdr
	{
		unsigned	int		offSet_adr;
		unsigned	int		lenght_adr;
	};
	//
	extern			unsigned	char			n_prog;
	extern			unsigned	char			c_prog;
	extern			unsigned	char			s_prog;
	extern	const	unsigned	char			blockBoundary;
	extern			unsigned	long			adrProg;
	extern			unsigned	char			mxMod;
	extern			unsigned	char			c_pos;
	extern			unsigned	char			statTarSector;
	// ================= дл€ записи на флэш =====================
	extern			unsigned	long			ml_adr_base;				// базовый адрес
	extern			unsigned	char			ml_adr_n;					// номер текущей программы
	extern	const	unsigned	char			ml_adr_bd_max;				// максимальное количество частей программы
	extern			StrucAdr					ml_adr_bd[];				// bd частей программы
	extern			unsigned	int				ml_adr_offset;				// относительный адрес
	extern			unsigned	int				ml_adr_offset_max;
	extern			unsigned	char			ml_adr_n_max;				// количество частей программы на выдачу
	extern			unsigned	char			read_fistEnter;				// первый пуск на чтение
	extern			unsigned	long			sendAdr;					// адрес данных дл€ передачи
	extern			unsigned	int				sendLenght;					// длина данных дл€ передачи
	extern			unsigned	char			flag_plus5 EEMEM;
	extern			unsigned	char			flag_eot   EEMEM;
	extern			unsigned	char			flag_plus5_m;
	extern			unsigned	char			flag_eot_m;
	extern			unsigned	char			pins_uk;
	//===========================================================
	struct ModStruct
	{
		unsigned	char	nMod;
		char	name[5];
	};
	
	
	extern	const	ModStruct	listMod[];
	extern	const char string_table[][10];

#ifdef		__DEBUG_SIMULYATION
	extern			unsigned	int				simRead_adr;				// адрес пам€ти дл€ симул€ции чтени€
	extern			unsigned	int				simRead_count_byte;			// счетчик дл€ симул€ции чтени€
	extern			unsigned	int				simRead_count_sproket;
#define			___SIM_READ_MAX_SPROKET			6
#endif

}


#endif //__VAR_H__
