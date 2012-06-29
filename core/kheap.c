#include <kheap.h>

extern u32i end;
u32i PlacementAddress = (u32i) &end;


u32i KmallocIntenal(u32i Size,s32i Align,u32i* Physical)
{
	if ( Align == 1 && (PlacementAddress & 0xFFFFF000)) {
		PlacementAddress &= 0xFFFFF000;
		PlacementAddress += 0x1000;
	} if ( Physical ) {
		*Physical = PlacementAddress;
	}
	u32i Temp = PlacementAddress;
	PlacementAddress += Size;
	return Temp;
}

u32i KmallocAligned(u32i Size)
{
	return KmallocIntenal(Size,0,0);
}

u32i KmallocPhysical(u32i Size,u32i* Physical)
{
	return KmallocIntenal(Size,1,Physical);
}

u32i KmallocAlignedPhysical(u32i Size ,u32i* Physical)
{
	return KmallocIntenal(Size,0,Physical);
}

u32i Kmalloc(u32i Size)
{
	return KmallocIntenal(Size,1,0);
}
