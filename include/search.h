#ifndef _SEARCH_H_
#define _SEARCH_H_

// State can be anything
typedef void *State;

// List of possible actions from state
typedef struct {
  // Number of actions in list
  unsigned int number_of_actions;
  // List
  void **actions;
} ActionsList;

// Problem definition
typedef struct {
  // Initial state of the problem
  State initial_state;
  
  // Function to test if a node is a goal,
  // receives the node as parameter and
  // returns 0 if it isn't or 1 if it is a goal
  unsigned char (*goal_test)(State);

  // Function that returns the actions to 
  // perform on the node passed as parameter
  ActionsList *((*actions)(State)); 
} Problem;

// Node element
typedef struct TNode {
  // Node state
  void *state;
  // Path cost of this node
  double path_cost;
  // Parent node to retrive path
  TNode *parent;
} Node;

// Any path, for example, a path to solution
typedef Node *Path;

#endif
