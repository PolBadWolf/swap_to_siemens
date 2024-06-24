/* 
* TaskQueueUnit.cpp
*
* Created: 10.04.2024 11:42:46
* Author: User
*/


#include "TaskQueueUnit.h"

// default constructor
TaskQueueUnit::TaskQueueUnit()
{
} //TaskQueueUnit

// default destructor
TaskQueueUnit::~TaskQueueUnit()
{
} //~TaskQueueUnit

TaskQueueUnit	TaskQueueUnit::crTask(TypeMetod metod, int8_t div_tik, uint8_t cur_tik)
{
	TaskQueueUnit	unit;
	unit.metod		= metod;
	unit.div_tik	= div_tik;
	unit.cur_tik	= cur_tik;
	return	unit;
}