#include <stdlib.h>
#include <pqueue.h>

// Create queue
pqueue_t
*pq_create_pqueue()
{
  pqueue_t *pqueue;

  pqueue = (pqueue_t*) malloc(sizeof(pqueue_t));

  pqueue->length = 0;
  pqueue->first = pqueue->last = NULL;

  return pqueue;
}

// Destroy entire queue
void
pq_destroy_pqueue (pqueue_t *queue)
{
  pqueuenode_t *node;

  node = queue->first;

  while (queue->first != NULL) {
    node = queue->first;
    queue->first = queue->first->next;
    free (node->data);
    free (node);
  }
  
  free (queue);
}

// Returns true if queue is empty
unsigned char
pq_is_empty (pqueue_t* queue)
{
  return (queue->length == 0);
}

// Returns true if element is in queue
unsigned char
pq_is_in_queue (void *element, unsigned char (*cmp_function)(void*, void*), pqueue_t *queue)
{
  return (pq_get(element, cmp_function, queue) != NULL);
} 

// Insert node in queue
void
pq_insert (void *data, double priority, pqueue_t *queue)
{
  pqueuenode_t *current, *last, *node;

  node = pq_create_pqueue_node (data, priority);

  if (pq_is_empty (queue)) {
    queue->first = queue->last = node;
    queue->length++;
    return;
  }

  if (node->priority < queue->first->priority) {
    node->next = queue->first;
    queue->first = node;
    queue->length++;
    return;
  }

  if (node->priority >= queue->last->priority) {
    queue->last->next = node;
    queue->last = node;
    queue->length++;
    return;
  }

  last = queue->first;
  current = queue->first->next;

  while (current->priority < node->priority) {
    last = current;
    current = current->next;
  }

  last->next = node;
  node->next = current;
  queue->length++;
}

// Remove node from queue
void
pq_remove_pqueue_node (void *data, unsigned char (*cmp_function)(void*, void*), pqueue_t *queue)
{
  pqueuenode_t *current, *last;

  if (pq_is_empty (queue)) {
    return;
  }

  if (cmp_function(data, queue->first->data)) {
    pq_pop(queue);
    return;
  }

  last = queue->first;
  current = queue->first->next;

  while (current != NULL) {
    if (cmp_function(data, current->data)) {
      last->next = current->next;

      if (current == queue->last) {
        queue->last = last;
      }

      queue->length--;

      free (current);
      return;
    }

    last = current;
    current = current->next;
  }
}

// Create pqueue node
pqueuenode_t
*pq_create_pqueue_node (void *data, double priority)
{
  pqueuenode_t *node;

  node = (pqueuenode_t*) malloc(sizeof(pqueuenode_t));

  node->priority = priority;
  node->data = data;
  node->next = NULL;

  return node;
}

// Get and remove node with lowest priority
void
*pq_pop (pqueue_t *queue)
{
  pqueuenode_t *node;
  void *data;
  
  node = queue->first;
  data = node->data;

  queue->first = queue->first->next;
  queue->length--;

  free (node);

  return data;
}

// Retrieve element in queue based in a compare function
void
*pq_get (void *element, unsigned char (*cmp_function)(void*, void*), pqueue_t *queue)
{
  pqueuenode_t *node;

  if (pq_is_empty(queue)) {
    return NULL;
  }

  node = queue->first;

  while (node != NULL) {

    if (cmp_function(element, node->data)) {
      return node->data;
    }

    node = node->next;
  }

  return NULL;
}

