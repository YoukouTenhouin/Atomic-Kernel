#include <heaplist.h>
#include <kheap.h>
#include <types.h>

HeapList
NewHeapList()
{
	HeapList Ret;
	Ret = AllocMemory(sizeof(HeapListEntry)*4097);
	Ret[0].IsHead = true;
	Ret[0].Next = 1;
	Ret[1].IsHead = false;
	Ret[1].Size = 
	return Ret;
}

HeapListPosition
FindHeapListEntry(HeapList List,u32i Address)
{
	while(List->Next!=NULL){
		List=List->Next;
		if(List!=NULL||List->MenRecord.Address==Address) return List;
	}
	return NULL;
}

DeleteHeapListEntry(HeapListEntry* ToDelete)
{
	HeapListEntry* Temp = ToDelete ->Next;
	ToDelete = Temp->Next;
	Free(Temp,sizeof(Temp));
}

void
InsertHeapListEntry(HeapListPosition Where,HeapListEntry* ToInsert)
{
	ToInsert->Next = Where->Next;
	Where-Next = ToInsert;
}
