#ifndef _SEARCH_H_
#define _SEARCH_H_

// State can be anything
typedef void *State;

// Node element
typedef struct TNode {
  // Node state
  State state;
  // Path cost of this node
  double path_cost;
  // Parent node to retrive path
  struct TNode *parent;
} Node;

// Action is a function which leads from a state into the next state
typedef Node *(*Action)(const State);

// Problem definition
typedef struct {
  // Initial state of the problem
  State initial_state;
  
  // Function to test if a node is a goal,
  // receives the node as parameter and
  // returns 0 if it isn't or 1 if it is a goal
  unsigned char (*goal_test)(Node*);

  // Function that returns the actions to 
  // perform on the node passed as parameter
  // It returns a function array NULL terminated
  Action *(*actions)(State); 
} Problem;

// Search function
void *A_star_search(Problem*, double (*)(Node*), unsigned char (*)(void*, void*), void *(*)(Node*));

// Create a single node
Node *create_node(State, double, Node*);

#endif
