#include <timer.h>
#include <isr.h>
#include <kstdlib/kio.h>
#include <assembly.h>

u32i Tick = 0;

static void TimerCallback(RegistersType Regs)
{
	Tick++;
	WriteString("Tick:");
	WriteNumber(Tick,10);
	NewLine();
}

void InitTimer(u32i Frequency)
{
	RegisterInterruptHandler(32,&TimerCallback);
	u32i Divisor = 1193180 / Frequency;
	outb(0x43,0x36);
	u8i Lower = (u8i)(Divisor & 0xFF);
	u8i Upper = (u8i)((Divisor>>8) & 0xFF);
	outb(0x40,Lower);
	outb(0x40,Upper);
}
