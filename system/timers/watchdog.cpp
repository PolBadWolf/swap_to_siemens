/* 
* watchdog.cpp
*
* Created: 13.02.2017 15:19:11
* Author: gumerovmn
*/

#include "config_sys.h"

#ifdef CONF_WATHDOG

#include "watchdog.h"

namespace ns_watchdog
{
    void Init(uint8_t timeCont)
    {
		wdt_enable(timeCont);
    }
    void DisabledCount()
    {
		wdt_disable();
    }
    void ResetCount()
    {
	    wdt_reset();
    }

} //watchdog
#endif // CONF_WATHDOG
