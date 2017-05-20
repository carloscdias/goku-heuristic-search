#ifndef _SEARCH_H_
#define _SEARCH_H_

#include <types.h>

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

// List of possible actions from state
typedef struct {
  // Number of actions in list
  unsigned int number_of_actions;
  // List of actions
  Action *actions;
} ActionsList;

// Problem definition
typedef struct {
  // Initial state of the problem
  State initial_state;
  
  // Function to test if a node is a goal,
  // receives the node as parameter and
  // returns 0 if it isn't or 1 if it is a goal
  byte (*goal_test)(State);

  // Function that returns the actions to 
  // perform on the node passed as parameter
  ActionsList *(*actions)(State); 
} Problem;

// Search function
Node *A_star_search(Problem*, double (*)(State), byte (*)(void*, void*));

// Create a single node
Node *create_node(State, double, Node*);

// Static function that generates a child node
static Node *make_child(Problem*, Node*, Action, double (*)(State));

#endif
