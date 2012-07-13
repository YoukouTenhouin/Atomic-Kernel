#include <kcommon.h>

void
KPanic(string PanicMessage)
{
	KPrintf("Panic:%s\n",PanicMessage);
	asm volatile ("hlt");
}

