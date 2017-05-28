#ifndef _PQUEUE_H_
#define _PQUEUE_H_

// Priority queue node
typedef struct pqueuenode {
  double priority;
  void *data;
  struct pqueuenode *next;
} pqueuenode_t;

// Priority queue
typedef struct {
  unsigned int length;
  pqueuenode_t *first, *last;
} pqueue_t;

// Create queue
pqueue_t *pq_create_pqueue();

// Destroy entire queue
void pq_destroy_pqueue(pqueue_t*);

// Returns true if queue is empty
unsigned char pq_is_empty(pqueue_t*);

// Returns true if element is in queue
unsigned char pq_is_in_queue(void*, unsigned char (*)(void*, void*), pqueue_t*); 

// Insert node in queue
void pq_insert(void*, double, pqueue_t*);

// Remove node from queue
void pq_remove_pqueue_node(void*, unsigned char (*)(void*, void*), pqueue_t*);

// Create pqueue node
pqueuenode_t *pq_create_pqueue_node(void*, double);

// Get and remove node with low priority
void *pq_pop(pqueue_t*);

// Retrieve element in queue based in a compare function
void *pq_get(void*, unsigned char (*)(void*, void*), pqueue_t*);

#endif
