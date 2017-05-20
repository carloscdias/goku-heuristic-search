#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

typedef struct TLinkedListNode {
  void *data;
  struct TLinkedListNode *next;
} LinkedListNode;

typedef struct TLinkedList {
  unsigned int number_of_elements;
  LinkedListNode *first_node, *last_node;
} LinkedList;

typedef unsigned char byte;

byte ll_is_empty(LinkedList *);

LinkedListNode *ll_create_linkedlist_node(void *);

LinkedList *ll_create_linkedlist();

void ll_insert_first_node(LinkedListNode *, LinkedList *);

void ll_insert_last_node(LinkedListNode *, LinkedList *);

LinkedListNode *ll_remove_first_node(LinkedList *);

LinkedListNode *ll_remove_last_node(LinkedList *);

#endif
