#include <stdlib.h>
#include <linkedlist.h>

// Returns true if list is empty
unsigned char
ll_is_empty (linkedlist_t *list) {
  return (list == NULL || list->first_node == NULL);
}

// Create linked list node
linkedlistnode_t
*ll_create_linkedlist_node (void *data)
{
  linkedlistnode_t *node;

  node = (linkedlistnode_t*) malloc(sizeof(linkedlistnode_t));
  node->data = data;
  node->next = NULL;

  return node;
}

// Create linked list
linkedlist_t
*ll_create_linkedlist()
{
  linkedlist_t *list;

  list = (linkedlist_t*) malloc(sizeof(linkedlist_t));
  list->first_node = list->last_node = NULL;
  list->number_of_elements = 0;

  return list;
}

// Insert first node in list
void
ll_insert_first_node (linkedlistnode_t *node, linkedlist_t *list)
{
  if (ll_is_empty (list)) {
    list->first_node = list->last_node = node;
  } else {
    node->next = list->first_node;
    list->first_node = node;
  }

  list->number_of_elements++;
}

// Inser last node in list
void
ll_insert_last_node (linkedlistnode_t *node, linkedlist_t *list)
{
  if (ll_is_empty (list)) {
    list->first_node = list->last_node = node;
  } else {
    list->last_node->next = node;
    list->last_node = node;
  }

  list->number_of_elements++;
}

// Remove first node from list
linkedlistnode_t
*ll_remove_first_node (linkedlist_t *list)
{
  linkedlistnode_t *node;

  if (ll_is_empty (list)) {
    return NULL;
  }

  node = list->first_node;
  list->first_node = list->first_node->next;
  list->number_of_elements--;

  if (ll_is_empty (list)) {
    list->last_node = NULL;
  }

  return node;
}

// Remove last node from list
linkedlistnode_t
*ll_remove_last_node (linkedlist_t *list)
{
  linkedlistnode_t *node, *tmp;

  if (ll_is_empty (list)) {
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

