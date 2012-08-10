/* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE */
/* Version 2, December 2004 */
/* Copyright (C) 2012 Chapaev <chapaev@fpscopycatresistance.org> */
/* Everyone is permitted to copy and distribute verbatim or modifiedcopies of this license document, and changing it is allowed as longas the name is changed. */
/* DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION */
/* 0. You just DO WHAT THE F**K YOU WANT TO. */

#ifndef	HEAP_IFCE_H
#define HEAP_IFCE_H

/* Function type */
typedef void	(*no_mem_handler_t)();
typedef void*	(*get_free_pages_t)(int);

/* Interfaces */
void* heap_malloc(int size);
void heap_free(void* address_to_free);
void heap_init(get_free_pages_t get_free_pages_func,no_mem_handler_t handler);

#endif	/* HEAP_IFCE_H */
