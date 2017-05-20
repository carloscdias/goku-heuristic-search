#ifndef _STACK_H_
#define _STACK_H_

#include <linkedlist.h>

typedef LinkedList* Stack;

Stack st_create_stack();

void st_destroy_stack(Stack);

void st_push(void *, Stack);

void *st_pop(Stack);

#endif
