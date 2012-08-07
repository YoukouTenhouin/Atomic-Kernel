/* heap.c --> 可移植的简单堆实现
 * By Chapaev */
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
#include "heap.h"

static void*	start;		/* Start of first page */
static void*	end;		/* End of last page */

static void*	first_block;	/* Start of the first block */
static void*	last_block;	/* End of last block */

static no_mem_handler_t		no_mem_handler;
static get_free_pages_t		get_free_pages;

typedef free_list_entry*	free_list_t;
typedef free_list_t		free_list_position;
typedef free_list_position	free_list_iterator;

free_list_t	free_list;

/* Get the size of the free block */
static inline int
size_of_free_block(free_list_entry* which_one)
{
	header_t* block = (void*)((char*)which_one - sizeof(header_t));
	return block->size;
}

/* Get the header of the free block */
static inline header_t*
header_of_free_block(free_list_entry* which_one)
{
	return which_one==NULL?NULL:(header_t*)(void*)((char*)which_one - sizeof(header_t));
}

/* Move list iterator a step forward */
static inline void
free_list_iterator_forward(free_list_iterator* to_move)
{
	*to_move = (*to_move)->next;
}

/* Insert a block to free list */
static void
free_list_insert(header_t* block_to_insert)
{
	free_list_position prev = free_list;
	/* Find previous one */
	while ( prev->next != NULL && size_of_free_block(prev->next) < block_to_insert->size )
		free_list_iterator_forward(&prev);
	/* entry to insert */
	free_list_entry* entry_to_insert = (void*)block_to_insert + sizeof(header_t);
	/* Insert it */
	entry_to_insert->next = prev->next;
	prev->next = entry_to_insert;
}

/* Delete a entry from free list */
static void
free_list_delete(free_list_position entry_to_delete)
{
	free_list_position prev = free_list;
	/* Find previous one */
	while( prev->next != NULL && prev->next != entry_to_delete )
		free_list_iterator_forward(&prev);
	/* No need to free memory */
	if ( NULL != prev->next )	/* If we found */
		prev->next = prev->next->next;
}

/* Find a block from free list and return the entry */
static free_list_entry*
free_list_find(header_t* to_find)
{
	free_list_iterator to_return = free_list;
	/* Loop until found or arrive end of free list */
	while( to_return != NULL && header_of_free_block(to_return) != to_find )
		free_list_iterator_forward(&to_return);
	/* If not find,to_return will be NULL         
	 * Else it will be the position of the header */
	return to_return;
}

/* Find smallest block can fill the size we need */
static header_t*
free_list_find_can_fill(int size)
{
	free_list_iterator can_fill = free_list;
	while ( can_fill != NULL && header_of_free_block(can_fill)->size < size )
		free_list_iterator_forward(&can_fill);
	return header_of_free_block(can_fill);
}

/* Get pages when out of memory */
/* Call handler until get free pages */
void*
oom_get_free_pages(int how_many)
{
	if ( no_mem_handler != NULL ){
		void* ret;
		while( NULL == ret ){ /* No free pages yet */
			no_mem_handler();
			ret = get_free_pages(how_many);
		} /* WE GOT FREE PAGES!!!! */
		return ret;
	}
	return NULL;		/* oops,No handler,i can do nothing for you */
}

/* Initialise func
 * Reigister functions and initialise a block */
void
heap_init(get_free_pages_t get_free_pages_func,
	  no_mem_handler_t handler)
{

	get_free_pages = get_free_pages_func;
	no_mem_handler = handler;
	/* Get one free page for first block */
	start = get_free_pages(1);
	/* No memory... */
	if ( NULL == start )
		start = oom_get_free_pages(1);
	/* End of heap */
	end = start + PAGE_SIZE;

	header_t* first_header = start + sizeof(free_list_entry);
	footer_t* first_footer = end - sizeof(footer_t);
	/* Initialise first block */
	*first_footer = first_header;
	first_header->size = PAGE_SIZE - sizeof(header_t) - sizeof(footer_t);
	first_header->in_use = 0; /* Of couse it not in use */
	first_block = first_header;
	last_block = (void*)first_footer + sizeof(footer_t);
	/* Now we initialise free list */
	free_list = start;
	free_list->next = (void*)first_header + sizeof(header_t);
	free_list->next->next = NULL;
}

/* Extend heap size
 * Return new block */
header_t*
extend_heap(u32 size_to_entend)
{
	/* Round up and change to page numbers */
	if (size_to_entend & 0xFFFFF000)
		size_to_entend = ((size_to_entend & 0xFFFFF000) / PAGE_SIZE) + 1;
	/* Get no pages */
	void* new_pages = get_free_pages(size_to_entend);
	/* oops,No free pages,try to get some */
	if ( NULL == new_pages )
		new_pages = oom_get_free_pages(size_to_entend);
	/* Create new block */
	header_t* new_header = new_pages;
	footer_t* new_footer = (void*)(new_header + PAGE_SIZE - sizeof(footer_t));
	/* A flat memory! */
	if ( end == new_pages ){
		footer_t* orig_footer = end - sizeof(footer_t);
		header_t* orig_header = *orig_footer;
		if(!(orig_header->in_use)){ /* How lucy we are! */
			new_header = orig_header;
			*new_footer = new_header;
		}
	}
	/* Now we construct new block */
	new_header->size = (void*)new_footer - (void*)new_header - sizeof(header_t);
	new_header->in_use = 0;
	/* Insert it to free list */
	free_list_insert(new_header);
	/* Update start and end */
	last_block = new_footer + sizeof(footer_t);
	
	return new_header;
}	

/* Spilt one block to two
 * Return first one */
header_t*
spilt_block(header_t* block_to_spilt,int size_of_first_block)
{
	/* Delete the block from free list first */
	free_list_delete(free_list_find(block_to_spilt));
	/* Spilt one to two */
	header_t* first_header = block_to_spilt;
	footer_t* first_footer = (void*)first_header + size_of_first_block + sizeof(header_t);
	header_t* second_header = (void*)first_footer + sizeof(footer_t);
	footer_t* second_footer = (void*)first_header + first_header->size + sizeof(header_t);
	/* Construct first block */
	*first_footer = first_header;
	first_header->size = size_of_first_block;
	/* Construct second block */
	*second_footer = second_header;
	second_header->size = (void*)second_footer - (void*)second_header - sizeof(header_t);
	second_header->in_use = 0;
	/* Insert them to free list */
	free_list_insert(first_header);
	free_list_insert(second_header);
	/* Return first one */
	return first_header;
}

/* Merge two blocks to one
 * Return block merged */
header_t*
merge_block(header_t* first_header,header_t* second_header)
{
	/* Delete both blocks */
	free_list_delete(free_list_find(first_header));
	free_list_delete(free_list_find(second_header));
	footer_t* second_footer = (void*)second_header + sizeof(header_t) + second_header->size;
	*second_footer = first_header;
	first_header->size = (void*)second_footer - (void*)first_header - sizeof(header_t);
	return first_header;
}	

/* Alloc memory */
void*
alloc_memory(int size)
{
	int new_size = size + sizeof(header_t) + sizeof(footer_t); /* Block size we need */
	header_t* header_to_use = free_list_find_can_fill(new_size); /* Find block */
	if ( NULL == header_to_use )	  /* oops,Out of memory */
		header_to_use = extend_heap(size);
	if ( header_to_use->size > size ) /* We need to spilt block */
		header_to_use = spilt_block(header_to_use,size);
	void* address_to_return = (void*)header_to_use + sizeof(header_t);
	/* The block is not free anymore */
	header_to_use->in_use = 1;
	free_list_delete(free_list_find(header_to_use));
	return address_to_return;
}

void
free_memory(void* address_to_free)
{
	/* Do some merge first */
	header_t* curr_header = address_to_free - sizeof(header_t);
	footer_t* curr_footer = (void*)curr_header + sizeof(header_t) + curr_header->size;
	header_t* next_header = (void*)curr_footer + sizeof(footer_t);
	footer_t* prev_footer = (void*)curr_header - sizeof(footer_t);
	header_t* prev_header = *prev_footer;
	/* Previous one isn't in use so we merge it */
	if ( first_block != (void*)curr_header && !(prev_header->in_use) ) 
		curr_header = merge_block(prev_header,curr_header);
	/* Same to next one */
	if ( last_block != (void*)curr_footer + sizeof(footer_t) && !(next_header->in_use) )
		merge_block(curr_header,next_header);
	/* This block isn't in use now */
	curr_header->in_use = 0;
	/* Insert it */
	free_list_insert(curr_header);
}

/* Now we define some interface */
void*
heap_malloc(int size)
{
	return alloc_memory(size);
}

void
heap_free(void* address_to_free)
{
	free_memory(address_to_free);
}
/* That's enough */
