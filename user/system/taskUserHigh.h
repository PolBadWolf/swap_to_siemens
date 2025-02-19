/*
 * taskUserHigh.h
 *
 * Created: 10.04.2024 15:21:02
 *  Author: User
 */ 


// TaskQueueUnit::crTask(Lcd_hard::Interrupt_static, 1 * ((double)timer0_FEQ / 1000.0), 0),
TaskQueueUnit::crTask(ReadData::timerFast_irq, ((double)timer0_FEQ / (double)READDATA_TimerFast), 0),
TaskQueueUnit::crTask(ns_user::writeData->timerFast_irq, ((double)timer0_FEQ / (double)WRITEDATA_TimerFast), 0),		// 2000 - 2000 тиков за секунду
