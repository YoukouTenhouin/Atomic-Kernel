#ifndef __ATOMIC_KCOMMON_H__
#define __ATOMIC_KCOMMON_H__

#include <kstdlib/kio.h>
#include <types.h>

void KPanic(string PanicMessage)
{
	WriteString("Panic:");
	WriteString(PanicMessage);
	NewLine();
	asm volatile ("hlt");
}

#endif //__ATOMIC_KHEAP_H__
