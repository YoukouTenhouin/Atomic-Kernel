#include <paging.h>
#include <kcommon.h>
#include <kheap.h>
#include <kstdlib/string.h>

PageDirectoryType* KernelDirectory = 0;
PageDirectoryType* CurrentDirectory = 0;

u32i* Frames;
u32i NFrames;

extern u32i PlacementAddress;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void SetFrame(u32i FrameAddress)
{
	u32i Frame = FrameAddress/0x1000;
	u32i Index = INDEX_FROM_BIT(Frame);
	u32i Offset = OFFSET_FROM_BIT(Frame);
	Frames[Index] |= (0x1 << Offset);
}

static void ClearFrame(u32i FrameAddress)
{
	u32i Frame = FrameAddress/0x1000;
	u32i Index = INDEX_FROM_BIT(Frame);
	u32i Offset = OFFSET_FROM_BIT(Frame);
	Frames[Index] &= ~(0x1 << Offset);
}

static u32i TestFrame(u32i FrameAddress)
{
	u32i Frame = FrameAddress/0x1000;
	u32i Index = INDEX_FROM_BIT(Frame);
	u32i Offset = OFFSET_FROM_BIT(Frame);
	return Frames[Index] & (0x1 << Offset);
}

static u32i FirstFrame()
{
	for ( u32i i = 0 ; i < INDEX_FROM_BIT(NFrames) ; i++ ){
		if ( Frames[1] != 0xFFFFFFFF ) {
			for ( u32i j = 0 ; j < 32 ; j++ ) {
				u32i ToTest = 0x1 << j;
				if ( !(Frames[i]&ToTest) ){
					return i*4*8+j;
				}
			}
		}
	}
}

void AllocFrame(PageType* Page,s32i IsKernel,s32i IsWriteable)
{
	if ( Page -> Frame == 0 ) {
		u32i Index = FirstFrame();
		if ( Index == (u32i)-1 )
			KPanic("No free frames");
		SetFrame(Index*0x1000);
		Page->Present = 1;
		Page->ReadWrite = (IsWriteable)?1:0;
		Page->User = (IsKernel)?0:1;
		Page->Frame = Index;
	}
}

void FreeFrame(PageType* Page)
{
	u32i Frame;
	if (Frame=Page->Frame) {
		ClearFrame(Frame);
		Page->Frame = 0;
	}
}

void InitialisePaging()
{
	u32i MemEndPage = 0x10000000;
	NFrames = MemEndPage / 0x1000;
	Frames = (u32i*)Kmalloc(INDEX_FROM_BIT(NFrames));
	memset(Frames,0,INDEX_FROM_BIT(NFrames));
	KernelDirectory = (PageDirectoryType*)KmallocAligned(sizeof(PageDirectoryType));
	memset(KernelDirectory,0,sizeof(PageDirectoryType));
	CurrentDirectory = KernelDirectory;
	for ( int i = 0 ; i < PlacementAddress ; i += 0x1000 )
		AllocFrame(GetPage(i,1,KernelDirectory),0,0);
	RegisterInterruptHandler(14,PageFault);
	SwitchPageDirectory(KernelDirectory);
}

void SwitchPageDirectory(PageDirectoryType* Directory)
{
	CurrentDirectory = Directory;
	asm volatile("mov %0,%%cr3"::"r"(&Directory->TablesPhysical));
	u32i cr0;
	asm volatile("mov %%cr0,%0":"=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0,%%cr0"::"r"(cr0));
}

PageType* GetPage(u32i Address,s32i Make,PageDirectoryType* Directory)
{
	Address /= 0x1000;
	u32i TableIndex = Address / 1024;
	if ( Directory->Tables[TableIndex] )
		return &Directory->Tables[TableIndex]->Pages[Address%1024];
	else if(Make) {
		u32i Temp;
		Directory->Tables[TableIndex] =
			(PageTableType*)KmallocAlignedPhysical(sizeof(PageTableType),&Temp);
		memset(Directory->Tables[TableIndex],0,0x1000);
		Directory->TablesPhysical[TableIndex] = Temp | 0x7;
		return &Directory->Tables[TableIndex]->Pages[Address%1024];
	} else return 0;
}

void PageFault(RegistersType Registers)
{
	u32i FaultingAddress;
	asm volatile("mov %%cr2,%0" : "=r" (FaultingAddress));
	int Present = ! (Registers.ErrorCode & 0x1);
	int ReadWrite = Registers.ErrorCode & 0x2;
	int User = Registers.ErrorCode & 0x4;
	int Reserved = Registers.ErrorCode & 0x8;
	int ID = Registers.ErrorCode & 10;
	WriteString("Page Fault at 0x");
	WriteNumber(FaultingAddress,16);
	WriteString("(");
	if ( Present ) WriteString("Present ");
	if ( ReadWrite ) WriteString("Read-only ");
	if ( User ) WriteString("User");
	if ( Reserved ) WriteString("Reserved");
	NewLine();
	KPanic("Page Fault");
}	
	
