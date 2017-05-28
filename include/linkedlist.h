#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

// Linked list node
typedef struct linkedlistnode {
  void *data;
  struct linkedlistnode *next;
} linkedlistnode_t;

// Linked list
typedef struct {
  unsigned int number_of_elements;
  linkedlistnode_t *first_node, *last_node;
} linkedlist_t;

// Check if list is empty
unsigned char ll_is_empty(linkedlist_t*);

// Create linked list node
linkedlistnode_t *ll_create_linkedlist_node(void*);

// Create a linked list
linkedlist_t *ll_create_linkedlist();

// Insert first node in a linkedlist
void ll_insert_first_node(linkedlistnode_t*, linkedlist_t*);

// Insert last node in a linkedlist
void ll_insert_last_node(linkedlistnode_t*, linkedlist_t*);

// Remove first node from a linked list
linkedlistnode_t *ll_remove_first_node(linkedlist_t*);

// Remove last node from a linked list
linkedlistnode_t *ll_remove_last_node(linkedlist_t*);

#endif
