#ifndef _PQUEUE_H_
#define _PQUEUE_H_

#include <types.h>

typedef struct TPQueueNode {
  double priority;
  void *data;
  struct TPQueueNode *next;
} PQueueNode;

typedef struct {
  unsigned int length;
  PQueueNode *first, *last;
} PQueue;

// Create queue
PQueue *create_pqueue();

// Destroy entire queue
void destroy_pqueue(PQueue*);

// Returns true if queue is empty
byte is_empty(PQueue*);

// Returns true if element is in queue
byte is_in_queue(void*, byte (*)(void*, void*), PQueue*); 

// Insert node in queue
void insert(void*, double, PQueue*);

// Remove node from queue
void remove_pqueue_node(void*, byte (*)(void*, void*), PQueue*);

// Create pqueue node
PQueueNode *create_pqueue_node(void*, double);

// Get and remove node with low priority
void *pop(PQueue*);

// Retrieve element in queue based in a compare function
void *get(void*, byte (*)(void*, void*), PQueue*);

#endif
