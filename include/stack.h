#ifndef _STACK_H_
#define _STACK_H_

#include <linkedlist.h>

// Stack data type
typedef linkedlist_t *stack_t;

// Create stack on the heap
stack_t st_create_stack();

// Destroy stack and stack data
void st_destroy_stack(stack_t);

// Push new element to the top of the stack
void st_push(void *, stack_t);

// Pop element from the top of the stack
void *st_pop(stack_t);

// Clear elements of stack
void st_clear_stack(stack_t);

#endif
