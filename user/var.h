/* 
* var.h
*
* Created: 14.05.2024 9:37:04
* Author: User
*/


#ifndef __VAR_H__
#define __VAR_H__

#include <avr/eeprom.h>

#define LIST_MOD_view		0
#define LIST_MOD_send		1
#define LIST_MOD_read		2
#define LIST_MOD_clr		3
#define LIST_MOD_copy		4
#define LIST_MOD_plus5		5
#define LIST_MOD_eot		6
#define LIST_MOD_pins		7

#define LIST_MOD_sd_minINT	8
#define LIST_MOD_sd_minSD	9

#define LIST_MOD_sd_plsINT	10
#define LIST_MOD_sd_plsFD	11
#define LIST_MOD_sd_plsSD	12

#define LIST_MOD__MAX		13

#define		OFFSET_WRITE	0x40


namespace	ns_var
{
	// === ��������� �������������� ������ � �����
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
	// ================= ��� ������ �� ���� =====================
	extern			unsigned	long			ml_adr_base;				// ������� �����
	extern			unsigned	char			ml_adr_n;					// ����� ������� ���������
	extern	const	unsigned	char			ml_adr_bd_max;				// ������������ ���������� ������ ���������
	extern			StrucAdr					ml_adr_bd[];				// bd ������ ���������
	extern			unsigned	int				ml_adr_offset;				// ������������� �����
	extern			unsigned	int				ml_adr_offset_max;
	extern			unsigned	char			ml_adr_n_max;				// ���������� ������ ��������� �� ������
	extern			unsigned	char			read_fistEnter;				// ������ ���� �� ������
	extern			unsigned	long			sendAdr;					// ����� ������ ��� ��������
	extern			unsigned	int				sendLenght;					// ����� ������ ��� ��������
	extern			unsigned	char			flag_plus5 EEMEM;
	extern			unsigned	char			flag_eot   EEMEM;
	extern			unsigned	char			flag_plus5_m;
	extern			unsigned	char			flag_eot_m;
	extern			unsigned	char			pins_uk;
	//===========================================================
	extern			unsigned	char			error_parity;
	extern			unsigned	char			fl_puskRead;
	extern			unsigned	char			fl_viewHex;
	
	struct ModStruct
	{
		unsigned	char	nMod;
		char	name[5];
	};
	
	
	extern	const	ModStruct	listMod[];
	extern	const char string_table[][10];

	extern			unsigned	char			simulOn;
	extern			unsigned	int				simulLenght;
	
	extern			unsigned	char			buf_string[];
	extern			unsigned	char			buf_string_adr;
	extern			unsigned	char			buf_string_lenght;
	
	extern			unsigned	char			waitEndCount;
	
	
	// ---------------------------
#define		safeDelay_minINT_d		1.0
#define		safeDelay_minSD_d		1.5
#define		safeDelay_plsINT_d		1.0
#define		safeDelay_plsFD_d		2.0
#define		safeDelay_plsSD_d		3.0
	// ---------------------------
	extern			uint8_t const	safeDelay_minINT_max;
	extern			uint8_t			safeDelay_minINT_e	EEMEM;
	
	extern			uint8_t const	safeDelay_minSD_max;
	extern			uint8_t			safeDelay_minSD_e	EEMEM;


	
	extern			uint8_t const	safeDelay_plsINT_max;
	extern			uint8_t			safeDelay_plsINT_e	EEMEM;
	
	extern			uint8_t const	safeDelay_plsFD_max;
	extern			uint8_t			safeDelay_plsFD_e	EEMEM;
	
	extern			uint8_t const	safeDelay_plsSD_max;
	extern			uint8_t			safeDelay_plsSD_e	EEMEM;
	// ---------------------------
	extern			uint8_t			safeDelay_minINT;
	extern			uint8_t			safeDelay_minSD;
	
	extern			uint8_t			safeDelay_plsINT;
	extern			uint8_t			safeDelay_plsFD;
	extern			uint8_t			safeDelay_plsSD;
	
	
	extern			uint8_t			edit8_tmp;

}


#endif //__VAR_H__
