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
	//
	const	ModStruct	listMod[]	PROGMEM =
	{
		{	0, "view"	},
		{	1, "save"	},
		{	2, "clr "	},
		{	3, "copy"	},
		{	4, "load"	},
		{	5, "R+5 "	},
		{	6, "EOT "	},
		{	7, "PINS"	},
	};
	/*const char string_table[][10] PROGMEM =
	{
		"String 1",
		"String 2",
		"String 3",
		"String 4",
		"String 5"
	};*/
	
	unsigned char	flag_plus5 EEMEM = 1;
	unsigned char	flag_eot   EEMEM = 1;
	unsigned char	flag_plus5_m;
	unsigned char	flag_eot_m;
	unsigned char	pins_uk;
}
