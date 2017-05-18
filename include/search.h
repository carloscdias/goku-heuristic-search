#ifndef _SEARCH_H_
#define _SEARCH_H_

// State can be anything
typedef void *State;

typedef struct {
  unsigned int number_of_actions;
  void **actions;
} ActionsList;

typedef struct {
  // Initial state of the problem
  State initial_state;
  
  // Function to test if a node is a goal,
  // receives the node as parameter and
  // returns 0 if it isn't or 1 if it is a goal
  unsigned char (*goal_test)(void*);

  // Function that returns the actions to 
  // perform on the node passed as parameter
  ActionsList *((*actions)(State)); 
} Problem;

typedef struct TNode {
  void *state;
  double path_cost;
  TNode *parent;
} Node;

#endif
