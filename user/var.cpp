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
