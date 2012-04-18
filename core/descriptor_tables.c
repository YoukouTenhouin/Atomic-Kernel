#include <descriptor_tables.h>

extern void GDTFlush(u32i);

static void InitGDT();
static void GDTSetGate(s32i,u32i,u32i,u8i,u8i);

GDTEntryType GDTEntries[5];
GDTPointerType GDTPointer;

void InitDescriptorTables()
{
	InitGDT();
}

static void InitGDT()
{
	GDTPointer.Limit = (sizeof(GDTEntryType)*5)-1;
	GDTPointer.Base = (u32i)&GDTEntries;
	GDTSetGate(0,0,0,0,0);
	GDTSetGate(1,0,0xffffffff,0x9a,0xcf);
	GDTSetGate(2,0,0xffffffff,0x92,0xcf);
	GDTSetGate(3,0,0xffffffff,0xfa,0xcf);
	GDTSetGate(4,0,0xffffffff,0xfa,0xcf);
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
	GDTEntries[Number].Granularity |= Gran & 0xfo;
	GDTEntries[Number].Access = Access;
}


