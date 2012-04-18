#include <isr.h>
#include <kstdlib/kio.h>

void
ISRHandler(RegistersType Regs)
{
	WriteString("Interrupt:");
	WriteNumber(Regs.IntNumber,10);
	WriteString("  ");
	WriteNumber(Regs.IntNumber,16);
	WriteString("\n");
}
