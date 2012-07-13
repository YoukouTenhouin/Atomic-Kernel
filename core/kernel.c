/* kernel.c --> 内核的入口 */
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

//#include <multiboot.h>
#include <types.h>
#include <kstdlib/kio.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <paging.h>

typedef struct {
	u32i MemoryInfo : 1;
	u32i BootDevice : 1;
	u32i CmdLine : 1;
	u32i Mods : 1;
	u32i Syms : 2;
	u32i MMap : 1;
	u32i Drives : 1;
	u32i ConfigTable : 1;
	u32i BootLoaderName : 1;
	u32i ApmTable : 1;
	u32i VBE : 1;
	u32i VoidFlags : 21;
} MultibootInfoFlags;

typedef struct
{
	u32i Lower;
	u32i Upper;
} MultibootMemtype;

typedef struct
{
	u8i Drive;
	u8i Part[3];
} MultibootBootDeviceType;

typedef struct
{
	u32i ModStart;
	u32i ModEnd;
	u32i String;
	u32i Reserved;
} MultibootModStruct;

typedef struct
{
	u32i ModsCount;
	MultibootModStruct* ModsAddress;
} MultibootModsType;

typedef struct
{
	u32i Length;
	u32i Address;
} MultibootMMapType;

typedef struct
{
	u32i Length;
	u32i Address;
} MultibootDrivesType;

typedef struct
{
	u32i ControlInfo;
	u32i ModeInfo;
	u32i Mode;
	u32i InterfaceSeg;
	u32i InterfaceOffset;
	u32i InterfaceLength;
} MultibootVBEMode;

typedef struct
{
	MultibootInfoFlags Flags;
	MultibootMemtype MemInfo;
	MultibootBootDeviceType BootDevice;
	u32i CmdLine;
	MultibootModsType Mods;
	u32i Syms[3];
	MultibootMMapType MMap;
	MultibootDrivesType Drives;
	u32i ConfigTable;
	u32i BootLoaderName;
	u32i ApmTable;
	MultibootVBEMode VBE;
} MultibootInfoStruct;

void InitKeyboard();

void
kmain(MultibootInfoStruct* MInfo,u32i magic )
{
	if( magic != 0x2BADB002 )
	{
		while(1);
	}
	InitDescriptorTables();
	ClearScreen();
	SetColor(BLACK,BRIGHT_WHITE);
	KPrintf("This is Atomic Kernel 12213A\nTest for KPrintf:\n");
	KPrintf("Dec:%d\nHex:%x\nStr:%s\nChar:%c\n",12345,0xc0ffee,"Hello World",'a');
	if (MInfo->Flags.MemoryInfo)
		KPrintf("Memory:Lower %x Upper %x\n",
			MInfo->MemInfo.Lower,MInfo->MemInfo.Upper);
	if (MInfo->Flags.BootLoaderName)
		KPrintf("BootLoader:%s\n",MInfo->BootLoaderName);
	if (MInfo->Flags.CmdLine)
		KPrintf("Kernel Args:%s\n",MInfo->CmdLine);
	KPrintf("Starting Timer...");
	InitTimer(1000);
	KPrintf("Done.\n");
	KPrintf("Starting Keyboard...");
	InitKeyboard();
	KPrintf("Done.\n");
	KPrintf("Starting Paging...");
	InitialisePaging();
	KPrintf("Done.\n");
	asm volatile ("sti");
	u32i* Pointer = (u32i*) 0xA0000000;
	u32i DoPageFault = *Pointer;
	while(1);
}

