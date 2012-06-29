#ifndef __ATOMIC_KHEAP_H__
#define __ATOMIC_KHEAP_H__

#include <types.h>

u32i KmallocIntenal(u32i Size,s32i Aligned,u32i* Physical);
u32i KmallocAligned(u32i Size);
u32i KmallocPhysical(u32i Size,u32i* Physical);
u32i KmallocAlignedPhysical(u32i Size,u32i* Physical);
u32i Kmalloc(u32i Size);

#endif //__ATOMIC_KHEAP_H__




















