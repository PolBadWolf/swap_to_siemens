/*
 * taskUserMidle.h
 *
 * Created: 10.04.2024 15:33:37
 *  Author: User
 */ 


// TaskQueueUnit::crTask(Lcd_hard::Interrupt_static, 1 * ((double)timer2_FEQ / 1000.0), 0),
// TaskQueueUnit::crTask(ns_user::flash->mx_irq, 1 * ((double)timer2_FEQ / 1000.0), 0),				// 1000 - 1000 тиков за секунду
TaskQueueUnit::crTask(ns_user::flash->mx_irq, 1 , 0),				// 1000 - 1000 тиков за секунду
TaskQueueUnit::crTask(ns_user::writeData->timerFast_irq, 1 * ((double)timer2_FEQ / 1000.0), 0),		// 2000 - 2000 тиков за секунду

