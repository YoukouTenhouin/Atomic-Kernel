#ifndef __ATOMIC_ASSEMBLY_H__
#define __ATOMIC_ASSEMBLY_H__

#include <types.h>

#define outb(port,value) __asm__ ( \
		"outb %%al, %%dx\n\t"::"al"(value),"dx"(port))

#define inb(port) (__extension__({		\
	unsigned char __res; \
	__asm__ ("inb \%%dx, %%al\n\t"		\
	 :"=a"(__res) \
	 :"dx"(port)); \
	__res;}))


#endif
