/* 
* New.h
*
* Created: 10.04.2024 13:21:36
* Author: User
*/


#ifndef __NEW_H__
#define __NEW_H__

#include <stdlib.h>

void *operator new(size_t);
void *operator new[](size_t);

void operator delete(void *);
void operator delete[](void *);

__extension__ typedef int __guard __attribute__((mode (__DI__)));
extern   int  __cxa_guard_acquire(__guard *);
extern   void __cxa_guard_release(__guard *);
extern   void __cxa_guard_abort  (__guard *);
extern   void __cxa_pure_virtual(void);


#endif //__NEW_H__
