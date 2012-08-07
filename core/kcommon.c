#include <kcommon.h>

void KPanic(string PanicMessage)
{
	WriteString("Panic:");
	WriteString(PanicMessage);
	NewLine();
	asm volatile ("hlt");
}

void Wait(boolean* WaitFor)
{
	while(!(*WaitFor));
}

