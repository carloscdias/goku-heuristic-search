#include <stdlib.h>
#include <stack.h>

// Create stack
m_stack_t
st_create_stack()
{
  m_stack_t stack;
  stack = (m_stack_t) ll_create_linkedlist();
  return stack;
}

// Destroy entire stack and its data
void
st_destroy_stack (m_stack_t stack)
{
  st_clear_stack (stack);
  free (stack);
}

// Push new element to top of the stack
void
st_push (void *data, m_stack_t stack)
{
  linkedlistnode_t *node;

  node = ll_create_linkedlist_node (data);
  ll_insert_first_node (node, stack);
}

// Pop element from top os the stack
void
*st_pop (m_stack_t stack)
{
  linkedlistnode_t *node;
  void *data;

  node = ll_remove_first_node (stack);
  data = node->data;

  free (node);

  return data;
}

// Clear stack elements
void
st_clear_stack (m_stack_t stack)
{
  void *data;

  while (!ll_is_empty (stack)) {
    data = st_pop (stack);
    free (data);
  }
}

