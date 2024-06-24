/* 
* TaskQueueUnit.h
*
* Created: 10.04.2024 11:42:46
* Author: User
*/


#ifndef __TASKQUEUEUNIT_H__
#define __TASKQUEUEUNIT_H__

#include "core/core_def.h"

class TaskQueueUnit
{
//variables
public:
	TypeMetod		metod;
	uint8_t			div_tik;
	uint8_t			cur_tik;
protected:
private:

//functions
public:
	TaskQueueUnit();
	~TaskQueueUnit();
	static	TaskQueueUnit	crTask(TypeMetod metod, int8_t div_tik, uint8_t cur_tik);
protected:
private:
// 	TaskQueueUnit( const TaskQueueUnit &c );
// 	TaskQueueUnit& operator=( const TaskQueueUnit &c );

}; //TaskQueueUnit

#endif //__TASKQUEUEUNIT_H__
