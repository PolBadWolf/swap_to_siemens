/* 
* menu.h
*
* Created: 15.04.2024 9:44:25
* Author: User
*/


#ifndef __MENU_H__
#define __MENU_H__

namespace	ns_menu
{
	void	init();
	void	mainCycle();
	void	interrupt();
	void	functMenu_aft(unsigned char mode, unsigned char stat);
	void	functMenu_pre(unsigned char mode, unsigned char stat);
}
	void	dump();

#endif //__MENU_H__
