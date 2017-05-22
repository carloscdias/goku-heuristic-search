#include <stdlib.h>
#include <stack.h>

Stack st_create_stack() {
  Stack stack;
  stack = (Stack) ll_create_linkedlist();
  return stack;
}

void st_destroy_stack(Stack stack) {
  void *data;

  while (!ll_is_empty(stack)) {
    data = st_pop(stack);
    free(data);
  }

  free(stack);
}

void st_push(void *data, Stack stack) {
  LinkedListNode *node;

  node = ll_create_linkedlist_node(data);
  ll_insert_first_node(node, stack);
}

void *st_pop(Stack stack) {
  LinkedListNode *node;
  void *data;

  node = ll_remove_first_node(stack);
  data = node->data;

  free(node);

  return data;
}

