#include <stdlib.h>
#include <linkedlist.h>

byte ll_is_empty(LinkedList *list) {
  return (list == NULL || list->first_node == NULL);
}

LinkedListNode *ll_create_linkedlist_node(void *data) {
  LinkedListNode *node;

  node = (LinkedListNode *) malloc(sizeof(LinkedListNode));
  node->data = data;
  node->next = NULL;

  return node;
}

LinkedList *ll_create_linkedlist() {
  LinkedList *list;

  list = (LinkedList *) malloc(sizeof(LinkedList));
  list->first_node = list->last_node = NULL;
  list->number_of_elements = 0;

  return list;
}

void ll_insert_first_node(LinkedListNode *node, LinkedList *list) {
  if (ll_is_empty(list)) {
    list->first_node = list->last_node = node;
  } else {
    node->next = list->first_node;
    list->first_node = node;
  }

  list->number_of_elements++;
}

void ll_insert_last_node(LinkedListNode *node, LinkedList *list) {
  if (ll_is_empty(list)) {
    list->first_node = list->last_node = node;
  } else {
    list->last_node->next = node;
    list->last_node = node;
  }

  list->number_of_elements++;
}

LinkedListNode *ll_remove_first_node(LinkedList *list) {
  LinkedListNode *node;

  if (ll_is_empty(list)) {
    return NULL;
  }

  node = list->first_node;
  list->first_node = list->first_node->next;
  list->number_of_elements--;

  if (ll_is_empty(list)) {
    list->last_node = NULL;
  }

  return node;
}

LinkedListNode *ll_remove_last_node(LinkedList *list) {
  LinkedListNode *node, *tmp;

  if (ll_is_empty(list)) {
    return NULL;
  }

  node = list->first_node;
  list->number_of_elements--;
  
  if (list->first_node == list->last_node) {
    list->first_node = list->last_node = NULL;
    return node;
  }

  while (node->next != list->last_node) {
    node = node->next;
  }

  tmp = list->last_node;
  node->next = NULL;
  list->last_node = node;

  return tmp;
}

