/* 
* New.cpp
*
* Created: 10.04.2024 13:21:35
* Author: User
*/


#include "New.h"
#include <stddef.h>

 void *operator new(size_t s)
 {
	 if (s == 0)	s = 1;
	 return malloc(s);
 }
 
 void *operator new[](size_t s)
 {
	 if (s == 0)	s = 1;
	 return malloc(s);
 }
 
 void operator delete(void *m)
 {
	 free(m);
 }
 
 void operator delete[](void *m)
 {
	 free(m);
 }
 