/*
 * menuUserTab.h
 *
 * Created: 15.04.2024 9:40:28
 *  Author: User
 */ 

//	0		:	view
// 1-4		:	push button
// 5		:	push multi button
// 6		:	init mode
// 7		:	timeout

//				view				key1				key2				key3				key4				m key				init			timeout
{		 start_count,		  start_key1,		  start_key2,		  start_key3,		  start_key4,				dump,		  start_init,				dump},		//	0
	// screen1
{	   screen1_check,		  screen1_k1,		  screen1_k2,		  screen1_k3,		  screen1_k4,				dump,		screen1_init,				dump},		//	1
	// copy
{				dump,			 copy_k1,			 copy_k2,			 copy_k3,			 copy_k4,				dump,		   copy_init,				dump},		//	2
	// clear
{				dump,			clear_no,			clear_no,		   clear_yes,				dump,				dump,		  clear_init,				dump},		//	3
	// reqeRead запуск чтения части программы (чтение с ленты)
{	  reqeRead_check,				dump,		 reqeRead_k2,		 reqeRead_k3,				dump,				dump,	   reqeRead_init,				dump},		//	4
	// чтение части программы
{	  readParty_view,				dump,				dump,				dump,		readParty_k4,				dump,	  readParty_init,				dump},		//	5
	// просмотр блока/сектора памяти
{	  viewBlock_view,		viewBlock_k1,		viewBlock_k2,		viewBlock_k3,		viewBlock_k4,				dump,	  viewBlock_init,				dump},		//	6
	// выбор части программы для записи
{	  reqeSend_check,				dump,		 reqeSend_k2,		 reqeSend_k3,				dump,				dump,	   reqeSend_init,				dump},		//	7
	// запись(отправка) части программы
{	  sendParty_view,	    sendParty_k1,				dump,				dump,				dump,				dump,	  sendParty_init,				dump},		//	8
	
{				dump,		leftRight_k1,		leftRight_k2,		leftRight_k3,		leftRight_k4,				dump,	  leftRight_init,				dump},		//	9
	
{				dump,			  eot_k1,			  eot_k2,			  eot_k3,			  eot_k4,				dump,			eot_init,				dump},		//	10
	
{				pins_view,		 pins_k1,				dump,				dump,				dump,				dump,		   pins_init,				dump},		//	11
	
{				dump,				dump,				dump,				dump,		  wt_scr1_go,				dump,				dump,				dump},		//	12
// =====================	
//	LIST_MOD_len_minus
{				dump,		len_minus_k1,		len_minus_k2,		len_minus_k3,		len_minus_k4,				dump,	  len_minus_init,				dump},		//	13
// =====================
{				dump,				dump,				dump,				dump,				dump,				dump,				dump,				dump},
{				dump,				dump,				dump,				dump,				dump,				dump,				dump,				dump},
{				dump,				dump,				dump,				dump,				dump,				dump,				dump,				dump},
