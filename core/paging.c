/* paging.c --> 分页相关物的实现 */
/* Copyright (c) 1998 著作权由Chapaev所有。著作权人保留一切权利。
 * 
 * 这份授权条款，在使用者符合以下三条件的情形下，授予使用者使用及再散播本
 * 软件包装原始码及二进位可执行形式的权利，无论此包装是否经改作皆然：
 * 
 * * 对于本软件源代码的再散播，必须保留上述的版权宣告、此三条件表列，以
 *   及下述的免责声明。
 * * 对于本套件二进位可执行形式的再散播，必须连带以文件以及／或者其他附
 *   于散播包装中的媒介方式，重制上述之版权宣告、此三条件表列，以及下述
 *   的免责声明。
 * * 未获事前取得书面许可，不得使用本软件贡献者之名称，
 *   来为本软件之衍生物做任何表示支持、认可或推广、促销之行为。
 * 
 * 免责声明：本软件是由本软件之贡献者以现状（"as is"）提供，
 * 本软件包装不负任何明示或默示之担保责任，包括但不限于就适售性以及特定目
 * 的的适用性为默示性担保。加州大学董事会及本软件之贡献者，无论任何条件、
 * 无论成因或任何责任主义、无论此责任为因合约关系、无过失责任主义或因非违
 * 约之侵权（包括过失或其他原因等）而起，对于任何因使用本软件包装所产生的
 * 任何直接性、间接性、偶发性、特殊性、惩罚性或任何结果的损害（包括但不限
 * 于替代商品或劳务之购用、使用损失、资料损失、利益损失、业务中断等等），
 * 不负任何责任，即在该种使用已获事前告知可能会造成此类损害的情形下亦然。*/

#include <paging.h>
#include <kcommon.h>
#include <kheap.h>
#include <kstdlib/string.h>
#include <kstdlib/kio.h>

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
		if ( Frames[i] != 0xFFFFFFFF ) {
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
	KPrintf("Page Fault at 0x%x (",FaultingAddress);
	if ( Present ) WriteString("Present ");
	if ( ReadWrite ) WriteString("Read-only ");
	if ( User ) WriteString("User");
	if ( Reserved ) WriteString("Reserved");
	KPrintf(")\n");
	KPanic("Page Fault");
}	
	
