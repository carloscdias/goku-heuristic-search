#ifndef _STACK_H_
#define _STACK_H_

#include <linkedlist.h>

// Stack data type
typedef linkedlist_t *m_stack_t;

// Create stack on the heap
m_stack_t st_create_stack();

// Destroy stack and stack data
void st_destroy_stack(m_stack_t);

// Push new element to the top of the stack
void st_push(void *, m_stack_t);

// Pop element from the top of the stack
void *st_pop(m_stack_t);

// Clear elements of stack
void st_clear_stack(m_stack_t);

#endif
