/* descriptor_tables.c --> GDT和IDT设置 */
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
