#include <stdlib.h>
#include <pqueue.h>

// Create queue
PQueue *create_pqueue() {
  PQueue *pqueue;

  pqueue = (PQueue*) malloc(sizeof(PQueue));

  pqueue->length = 0;
  pqueue->first = pqueue->last = NULL;

  return pqueue;
}

// Destroy entire queue
void destroy_pqueue(PQueue *queue) {
  PQueueNode *node;

  node = queue->first;

  while(queue->first != NULL) {
    node = queue->first;
    queue->first = queue->first->next;
    free(node->data);
    free(node);
  }
  
  free(queue);
}

// Returns true if queue is empty
byte is_empty(PQueue* queue) {
  return (queue->length == 0);
}

// Returns true if element is in queue
byte is_in_queue(void *element, byte (*cmp_function)(void*, void*), PQueue *queue) {
  return (get(element, cmp_function, queue) != NULL);
} 

// Insert node in queue
void insert(void *data, double priority, PQueue *queue) {
  PQueueNode *current, *last, *node;

  node = create_pqueue_node(data, priority);

  if (is_empty(queue)) {
    queue->first = queue->last = node;
    queue->length++;
    return;
  }

  if(node->priority < queue->first->priority) {
    node->next = queue->first;
    queue->first = node;
    queue->length++;
    return;
  }

  if(node->priority >= queue->last->priority) {
    queue->last->next = node;
    queue->last = node;
    queue->length++;
    return;
  }

  last = queue->first;
  current = queue->first->next;

  while(current->priority < node->priority) {
    last = current;
    current = current->next;
  }

  last->next = node;
  node->next = current;
  queue->length++;
}

// Remove node from queue
void remove_pqueue_node(void *data, byte (*cmp_function)(void*, void*), PQueue *queue) {
  PQueueNode *current, *last;

  if(is_empty(queue)) {
    return;
  }

  if(cmp_function(data, queue->first->data)) {
    pop(queue);
    return;
  }

  last = queue->first;
  current = queue->first->next;

  while(current != NULL) {
    if(cmp_function(data, current->data)) {
      last->next = current->next;

      if(current == queue->last) {
        queue->last = last;
      }

      free(current);
      return;
    }

    last = current;
    current = current->next;
  }
}

// Create pqueue node
PQueueNode *create_pqueue_node(void *data, double priority) {
  PQueueNode *node;

  node = (PQueueNode*) malloc(sizeof(PQueueNode));

  node->priority = priority;
  node->data = data;
  node->next = NULL;

  return node;
}

// Get and remove node with lowest priority
void *pop(PQueue *queue) {
  PQueueNode *node;
  void *data;
  
  node = queue->first;
  data = node->data;

  queue->first = queue->first->next;
  queue->length--;

  free(node);

  return data;
}

// Retrieve element in queue based in a compare function
void *get(void *element, byte (*cmp_function)(void*, void*), PQueue *queue) {
  PQueueNode *node;

  if (is_empty(queue)) {
    return NULL;
  }

  node = queue->first;

  while(node != NULL) {

    if(cmp_function(element, node->data)) {
      return node->data;
    }

    node = node->next;
  }

  return NULL;
}

