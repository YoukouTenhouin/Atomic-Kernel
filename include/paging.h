#ifndef __ATOMIC_PAGING_H__
#define __ATOMIC_PAGING_H__

#include <isr.h>

typedef struct Page
{
	u32i Present : 1;
	u32i ReadWrite : 1;
	u32i User : 1;
	u32i Accessed : 1;
	u32i Dirty : 1;
	u32i Unused : 7;
	u32i Frame:20;
} PageType;

typedef struct PageTable
{
	PageType Pages[1024];
} PageTableType;

typedef struct PageDirectory
{
	PageTableType* Tables[1024];
	u32i TablesPhysical[1024];
	u32i PhysicalAddress;
} PageDirectoryType;

void InitialisePaging();
void SwitchPageDirectory(PageDirectoryType* NewDirectory);
PageType* GetPage(u32i Address,int Make,PageDirectoryType* Directory);
void PageFault(RegistersType Registers);

#endif //__ATOMIC_PAGING_H__
	
