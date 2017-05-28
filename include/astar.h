#ifndef _ASTAR_H_
#define _ASTAR_H_

// State can be anything
typedef void *state_t;

// Node element
typedef struct node {
  // Node state
  state_t state;
  // Path cost of this node
  double path_cost;
  // Parent node to retrive path
  struct node *parent;
} node_t;

// Action is a function which leads from a state into the next state
typedef node_t *(*action_t)(const state_t);

// Problem definition
typedef struct problem {
  // Initial state of the problem
  state_t initial_state;

  // State for problems where the the final
  // state is known and constant
  state_t final_state;
  
  // Function to test if a node is a goal,
  // receives the problem and the node state 
  // as parameter and returns 0 if it isn't
  // or 1 if it is a goal
  unsigned char (*goal_test)(struct problem*, state_t);

  // Function that returns the actions to 
  // perform on the node passed as parameter
  // It returns a function array NULL terminated
  action_t *(*actions)(state_t); 
} problem_t;

// Search function
void *as_a_star_search(problem_t*, double (*)(problem_t*, state_t), unsigned char (*)(void*, void*), void *(*)(node_t*));

// Create a single node
node_t *as_create_node(state_t, double, node_t*);

#endif
