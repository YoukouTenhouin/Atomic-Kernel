#include <isr.h>
#include <kstdlib/kio.h>
#include <assembly.h>

ISRType InterruptHandlers[256];

void RegisterInterruptHandler(u8i n,ISRType Handler)
{
	InterruptHandlers[n] = Handler ;
}


void
ISRHandler(RegistersType Regs)
{
	WriteString("Interrupt:");
	WriteNumber(Regs.IntNumber,10);
	WriteString("  ");
	WriteNumber(Regs.IntNumber,16);
	WriteString("\n");
}

void
IRQHandler(RegistersType Regs)
{
	if (Regs.IntNumber >= 40) {
		outb(0xa0,0x20);
	}
	outb(0x20,0x20);
	if (InterruptHandlers[Regs.IntNumber] != 0) {
			ISRType Handler = InterruptHandlers[Regs.IntNumber];
			Handler(Regs);
	}
}
