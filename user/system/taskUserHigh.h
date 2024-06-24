/*
 * taskUserHigh.h
 *
 * Created: 10.04.2024 15:21:02
 *  Author: User
 */ 


// TaskQueueUnit::crTask(Lcd_hard::Interrupt_static, 1 * ((double)timer0_FEQ / 1000.0), 0),
TaskQueueUnit::crTask(ReadData::timerFast_irq, 1, 0),
