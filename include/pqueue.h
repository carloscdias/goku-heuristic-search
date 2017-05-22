#ifndef _PQUEUE_H_
#define _PQUEUE_H_

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
unsigned char is_empty(PQueue*);

// Returns true if element is in queue
unsigned char is_in_queue(void*, unsigned char (*)(void*, void*), PQueue*); 

// Insert node in queue
void insert(void*, double, PQueue*);

// Remove node from queue
void remove_pqueue_node(void*, unsigned char (*)(void*, void*), PQueue*);

// Create pqueue node
PQueueNode *create_pqueue_node(void*, double);

// Get and remove node with low priority
void *pop(PQueue*);

// Retrieve element in queue based in a compare function
void *get(void*, unsigned char (*)(void*, void*), PQueue*);

#endif
