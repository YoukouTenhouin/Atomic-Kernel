#include <descriptor_tables.h>
#include <kstdlib/string.h>

extern void GDTFlush(u32i);
extern void IDTFlush(u32i);

static void InitGDT();
static void GDTSetGate(s32i,u32i,u32i,u8i,u8i);
static void InitIDT();
static void IDTSetGate(u8i,u32i,u16i,u8i);

GDTEntryType GDTEntries[5];
GDTPointerType GDTPointer;

IDTEntryType IDTEntries[256];
IDTPointerType IDTPointer;


void InitDescriptorTables()
{
	InitGDT();
	InitIDT();
}

static void InitGDT()
{
	GDTPointer.Limit = (sizeof(GDTEntryType)*5)-1;
	GDTPointer.Base = (u32i)&GDTEntries;
	GDTSetGate(0,0,0,0,0);
	GDTSetGate(1,0,0xffffffff,0x9a,0xcf);
	GDTSetGate(2,0,0xffffffff,0x92,0xcf);
	GDTSetGate(3,0,0xffffffff,0xfa,0xcf);
	GDTSetGate(4,0,0xffffffff,0xf2,0xcf);
	GDTFlush((u32i)&GDTPointer);
}

static void GDTSetGate(s32i Number,u32i Base,
		       u32i Limit,u8i Access,u8i Gran)
{
	GDTEntries[Number].BaseLow = (Base & 0xffff);
	GDTEntries[Number].BaseMiddle = (Base>>16)&0xff;
	GDTEntries[Number].BaseHigh = (Base>>24)&0xff;
	GDTEntries[Number].LimitLow = (Limit&0xffff);
	GDTEntries[Number].Granularity = (Limit>>16)&0x0f;
	GDTEntries[Number].Granularity |= Gran & 0xf0;
	GDTEntries[Number].Access = Access;
}

static void InitIDT()
{
	IDTPointer.Limit = sizeof(IDTEntryType)*256-1;
	IDTPointer.Base = (u32i)&IDTEntries;
	memset(&IDTEntries,0,sizeof(IDTEntryType)*256);
	IDTSetGate(0,(u32i)isr0,0x08,0x8e);
	IDTSetGate(1,(u32i)isr1,0x08,0x8e);
	IDTSetGate(2,(u32i)isr2,0x08,0x8e);
	IDTSetGate(3,(u32i)isr3,0x08,0x8e);
	IDTSetGate(4,(u32i)isr4,0x08,0x8e);
	IDTSetGate(5,(u32i)isr5,0x08,0x8e);
	IDTSetGate(6,(u32i)isr6,0x08,0x8e);
	IDTSetGate(7,(u32i)isr7,0x08,0x8e);
	IDTSetGate(8,(u32i)isr8,0x08,0x8e);
	IDTSetGate(9,(u32i)isr9,0x08,0x8e);
	IDTSetGate(10,(u32i)isr10,0x08,0x8e);
	IDTSetGate(11,(u32i)isr11,0x08,0x8e);
	IDTSetGate(12,(u32i)isr12,0x08,0x8e);
	IDTSetGate(13,(u32i)isr13,0x08,0x8e);
	IDTSetGate(14,(u32i)isr14,0x08,0x8e);
	IDTSetGate(15,(u32i)isr15,0x08,0x8e);
	IDTSetGate(16,(u32i)isr16,0x08,0x8e);
	IDTSetGate(17,(u32i)isr17,0x08,0x8e);
	IDTSetGate(18,(u32i)isr18,0x08,0x8e);
	IDTSetGate(19,(u32i)isr19,0x08,0x8e);
	IDTSetGate(20,(u32i)isr20,0x08,0x8e);
	IDTSetGate(21,(u32i)isr21,0x08,0x8e);
	IDTSetGate(22,(u32i)isr22,0x08,0x8e);
	IDTSetGate(23,(u32i)isr23,0x08,0x8e);
	IDTSetGate(24,(u32i)isr24,0x08,0x8e);
	IDTSetGate(25,(u32i)isr25,0x08,0x8e);
	IDTSetGate(26,(u32i)isr26,0x08,0x8e);
	IDTSetGate(27,(u32i)isr27,0x08,0x8e);
	IDTSetGate(28,(u32i)isr28,0x08,0x8e);
	IDTSetGate(29,(u32i)isr29,0x08,0x8e);
	IDTSetGate(30,(u32i)isr30,0x08,0x8e);
	IDTSetGate(31,(u32i)isr31,0x08,0x8e);
	IDTFlush((u32i)&IDTPointer);
}

static void IDTSetGate(u8i Number,u32i Base,u16i Selector,u8i Flags)
{
	IDTEntries[Number].BaseLow = Base & 0xffff;
	IDTEntries[Number].BaseHigh = (Base>>16)&0xffff;
	IDTEntries[Number].Selector = Selector;
	IDTEntries[Number].Always0 = 0;
	IDTEntries[Number].Flags = Flags;
}
