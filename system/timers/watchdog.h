/* 
* watchdog.h
*
* Created: 13.02.2017 15:19:12
* Author: gumerovmn
*/


#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include "core/core_def.h"
#include <avr/wdt.h>

namespace ns_watchdog
{
    void Init(uint8_t timeCont);
    void DisabledCount();
    void ResetCount();
} //watchdog

#endif //__WATCHDOG_H__
