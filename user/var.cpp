/* 
* var.cpp
*
* Created: 14.05.2024 9:37:04
* Author: User
*/


#include "var.h"
#include <avr/pgmspace.h>

namespace	ns_var
{
			unsigned	char			n_prog;						// текущий блок/сектор
			unsigned	char			c_prog;						// блок/сектор копирования
			unsigned	char			s_prog;						// статус программы: блок=1 / сектор=0
			unsigned	char			c_pos;						// позитция курсора для вывода
	const	unsigned	char			blockBoundary		= 4;	// количество блоков
			unsigned	long			adrProg;					// текущий адрес блока/сектора
			unsigned	char			mxMod				= 0;	// режим работы: копирование, стирание, чтение, запись
			unsigned	char			statTarSector;				// статус целевого блока/сектора (0 - разрешено копирование)
			// для записи во флеш
			unsigned	long			ml_adr_base;				// базовый адрес
			unsigned	char			ml_adr_n;					// номер текущей части программы
	const	unsigned	char			ml_adr_bd_max = ((OFFSET_WRITE / 4) - 1);	// максимальное количество частей программы
			StrucAdr					ml_adr_bd[ml_adr_bd_max];	// bd частей программы
			unsigned	int				ml_adr_offset;				// относительный адрес записываемой части
			unsigned	char			ml_view;					// флаг отображения
			unsigned	int				ml_adr_offset_max;
			unsigned	char			ml_adr_n_max;				// количество частей программы на выдачу
			unsigned	char			read_fistEnter;				// первый пуск на чтение
			unsigned	long			sendAdr;					// адрес данных для передачи
			unsigned	int				sendLenght;					// длина данных для передачи
			
			unsigned	char			error_parity;
			unsigned	char			fl_puskRead;
			
			unsigned	char			simulOn;
			unsigned	int				simulLenght;
			
			unsigned	char			buf_string[64];
			unsigned	char			buf_string_adr;
			unsigned	char			buf_string_lenght;
			unsigned	char			fl_viewHex;
			unsigned	char			back_lenght;
	//
	const	ModStruct	listMod[]	PROGMEM =
	{
		{	0, "view"	},
		{	1, "send"	},
		{	2, "read"	},
		{	3, "clr "	},
		{	4, "copy"	},
		{	5, "R+5 "	},
		{	6, "EOT "	},
		{	7, "PINS"	},
	};
	
	unsigned char	flag_plus5 EEMEM = 0;
	unsigned char	flag_eot   EEMEM = 0;
	unsigned char	flag_plus5_m;
	unsigned char	flag_eot_m;
	unsigned char	pins_uk;
}
