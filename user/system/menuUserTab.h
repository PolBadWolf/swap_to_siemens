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
{		 start_count,		  start_key1,		  start_key2,		  start_key3,		  start_key4,				dump,		  start_init,				dump},
	// screen1
{	   screen1_check,		  screen1_k1,		  screen1_k2,		  screen1_k3,		  screen1_k4,				dump,		screen1_init,				dump},
	// copy
{				dump,			 copy_k1,			 copy_k2,			 copy_k3,			 copy_k4,				dump,		   copy_init,				dump},
	// clear
{				dump,			clear_no,			clear_no,		   clear_yes,				dump,				dump,		  clear_init,				dump},
	// reqeRead запуск чтения части программы (чтение с ленты)
{				dump,				dump,		 reqeRead_k2,		 reqeRead_k3,				dump,				dump,	   reqeRead_init,				dump},
	// чтение части программы
{	  readParty_view,				dump,				dump,				dump,		readParty_k4,				dump,	  readParty_init,				dump},
	// просмотр блока/сектора памяти
{	  viewBlock_view,		viewBlock_k1,		viewBlock_k2,		viewBlock_k3,		viewBlock_k4,				dump,	  viewBlock_init,				dump},
	// выбор части программы для записи
{	  reqeSend_check,				dump,		 reqeSend_k2,		 reqeSend_k3,				dump,				dump,	   reqeSend_init,				dump},
	// запись(отправка) части программы
{	  sendParty_view,	    sendParty_k1,				dump,				dump,				dump,				dump,	  sendParty_init,				dump},
	
{				dump,			plus5_k1,			plus5_k2,			plus5_k3,			plus5_k4,				dump,		  plus5_init,				dump},
	
{				dump,			  eot_k1,			  eot_k2,			  eot_k3,			  eot_k4,				dump,			eot_init,				dump},
	
{				pins_view,		 pins_k1,				dump,				dump,				dump,				dump,		   pins_init,				dump},
	
{				dump,				dump,				dump,				dump,		  wt_scr1_go,				dump,				dump,				dump},
	
{				dump,				dump,				dump,				dump,				dump,				dump,				dump,				dump},
{				dump,				dump,				dump,				dump,				dump,				dump,				dump,				dump},
{				dump,				dump,				dump,				dump,				dump,				dump,				dump,				dump},
