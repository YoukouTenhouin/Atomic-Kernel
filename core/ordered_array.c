#include <ordered_array.h>
#include <kheap.h>

s8i
StandardLessthanPredicate(TypeType a,TypeType b)
{
	return (a<b)?1:0;
}

OrderedArrayType
CreateOrderedArray(u32i MaxSize,LessthanPredicateType Lessthan)
{
	OrderedArrayType ToReturn;
	ToReturn.Array = (void*)Kmalloc(MaxSize*sizeof(TypeType));
	memset(ToReturn.Array,0,MaxSize*sizeof(TypeType));
	ToReturn.Size = 0;
	ToReturn.MaxSize = MaxSize;
	ToReturn.Lessthan = Lessthan;
	return ToReturn;
}

OrderedArrayType
PlaceOrderedArray(void* Address,u32i MaxSize,LessthanPredicateType Lessthan)
{
	OrderedArrayType ToReturn;
	ToReturn.Array = (TypeType*)Address;
	memset(ToReturn.Array,0,MaxSize*sizeof(TypeType));
	ToReturn.Size = 0;
	ToReturn.MaxSize = MaxSize;
	ToReturn.Lessthan = Lessthan;
	return ToReturn;
}

void
DestroyOrderedArray(OrderedArrayType* Array)
{
	//kfree(array->Array);
}

void
InsertOrderedArray(TypeType Item,OrderedArrayType* Array)
{
	ASSERT(Array->Lessthan);
	u32i Iterator;
	for(Iterator=0;Iterator<Array->Size&&Array->Lessthan(Array->Array[Iterator],Item));;
	if (Iterator==Array->Size)
		Array->Array[Array->Size] = Item;
	else{
		TypeType Temp = Array->Array[Iterator];
		Array->Array[Iterator] = Item;
		while(Iterator<Array->Size){
			Iterator++;
			TypeType Temp2 = Array->Array[Iterator];
			Array->Array[Iterator]=Temp;
			Temp = Temp2;
		}
		Array->Size++;
	}
}


TypeType
LookUpOrderedArray(u32i I,OrderedArrayType* Array)
{
	ASSERT(I < Array->Size);
	return Array->Array[i];
}

void
RemoveOrderedArray(u32i i,OrderedArrayType* Array)
{
	while(i<Array->Size){
		Array->Array[i] = Array->Array[i+1];
		i++;
	}
	Array->Size--;
}
