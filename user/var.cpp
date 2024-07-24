/* 
* var.cpp
*
* Created: 14.05.2024 9:37:04
* Author: User
*/


#include "var.h"
#include <avr/pgmspace.h>
#include "core/core_timers.h"

namespace	ns_var
{
			unsigned	char			n_prog;						// ������� ����/������
			unsigned	char			c_prog;						// ����/������ �����������
			unsigned	char			s_prog;						// ������ ���������: ����=1 / ������=0
			unsigned	char			c_pos;						// �������� ������� ��� ������
	const	unsigned	char			blockBoundary		= 4;	// ���������� ������
			unsigned	long			adrProg;					// ������� ����� �����/�������
			unsigned	char			mxMod				= 0;	// ����� ������: �����������, ��������, ������, ������
			unsigned	char			statTarSector;				// ������ �������� �����/������� (0 - ��������� �����������)
			// ��� ������ �� ����
			unsigned	long			ml_adr_base;				// ������� �����
			unsigned	char			ml_adr_n;					// ����� ������� ����� ���������
	const	unsigned	char			ml_adr_bd_max = ((OFFSET_WRITE / 4) - 1);	// ������������ ���������� ������ ���������
			StrucAdr					ml_adr_bd[ml_adr_bd_max];	// bd ������ ���������
			unsigned	int				ml_adr_offset;				// ������������� ����� ������������ �����
			unsigned	char			ml_view;					// ���� �����������
			unsigned	int				ml_adr_offset_max;
			unsigned	char			ml_adr_n_max;				// ���������� ������ ��������� �� ������
			unsigned	char			read_fistEnter;				// ������ ���� �� ������
			unsigned	long			sendAdr;					// ����� ������ ��� ��������
			unsigned	int				sendLenght;					// ����� ������ ��� ��������
			
			unsigned	char			error_parity;
			unsigned	char			fl_puskRead;
			
			unsigned	char			simulOn;
			unsigned	int				simulLenght;
			
			uint8_t						buf_string[64];
			uint8_t						buf_string_adr;
			uint8_t						buf_string_lenght;
			uint8_t						fl_viewHex;
			uint8_t						back_lenght;
			
			uint8_t						waitEndCount;
	//
	const	ModStruct	listMod[]	PROGMEM =
	{
		{	0, "view"	},
		{	1, "send"	},
		{	2, "read"	},
		{	3, "clr "	},
		{	4, "copy"	},
		{	5, "R+5 "	},
		{	6, "test"	},
		{	7, "PINS"	},
		{	8, "-int"	},
		{	9, "-SD "	},
		{  10, "+int"	},
		{  11, "+FD "	},
		{  12, "+SD "	},
	};
	
	uint8_t			flag_plus5 EEMEM = 0;
	uint8_t			flag_eot   EEMEM = 0;
	uint8_t			flag_plus5_m;
	uint8_t			flag_eot_m;
	uint8_t			pins_uk;
	// ---------------------------
	uint8_t const	safeDelay_minINT_max		= (uint8_t)(((double)safeDelay_minINT_d) * timer0_FEQ / 1000);
	uint8_t			safeDelay_minINT_e	EEMEM	= (uint8_t)(((double)0.5) * timer0_FEQ / 1000);
	
	uint8_t const	safeDelay_minSD_max			= (uint8_t)(((double)safeDelay_minSD_d) * timer0_FEQ / 1000);
	uint8_t			safeDelay_minSD_e	EEMEM	= (uint8_t)(((double)0.75) * timer0_FEQ / 1000);



	uint8_t const	safeDelay_plsINT_max		= (uint8_t)(((double)safeDelay_plsINT_d) * timer0_FEQ / 1000);
	uint8_t			safeDelay_plsINT_e	EEMEM	= (uint8_t)(((double)0.5) * timer0_FEQ / 1000);
	
	uint8_t const	safeDelay_plsFD_max			= (uint8_t)(((double)safeDelay_plsFD_d) * timer0_FEQ / 1000);
	uint8_t			safeDelay_plsFD_e	EEMEM	= (uint8_t)(((double)0.25) * timer0_FEQ / 1000);
	
	uint8_t const	safeDelay_plsSD_max			= (uint8_t)(((double)safeDelay_plsSD_d) * timer0_FEQ / 1000);
	uint8_t			safeDelay_plsSD_e	EEMEM	= (uint8_t)(((double)0.5) * timer0_FEQ / 1000);
	// ---------------------------
	uint8_t			safeDelay_minINT;
	uint8_t			safeDelay_minSD;

	uint8_t			safeDelay_plsINT;
	uint8_t			safeDelay_plsFD;
	uint8_t			safeDelay_plsSD;
	
	// ---------------------------
	uint8_t			edit8_tmp;
}
