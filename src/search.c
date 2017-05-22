#include <stdlib.h>
#include <search.h>
#include <pqueue.h>

// Static function that generates a child node
static Node *make_child(Problem*, Node*, Action);

// Static function to compare nodes based on node state
static unsigned char compare_nodes(void*, void*);

// Global to store compare function
// based in state received in search
unsigned char (*_cmp_function)(void*, void*);

// Create node
Node *create_node(State state, double cost, Node* parent) {
  Node *node;

  node = (Node *) malloc(sizeof(Node));

  node->state = state;
  node->path_cost = cost;
  node->parent = parent;

  return node;
}

// A function to compare states receiving nodes
unsigned char compare_nodes(void *node1, void *node2) {
  return _cmp_function(((Node*)node1)->state, ((Node*)node2)->state);
}

// A Star algorithm
void *A_star_search(Problem *problem, double (*h)(State), unsigned char (*cmp_state_function)(void*, void*), void *(*solution)(Node*)){
  Node *node, *child, *in_frontier;
  PQueue *frontier, *explored;
  Action *actions_list;
  void *memo_solution;
  unsigned int i;

  _cmp_function = cmp_state_function;

  // priority queue data structure
  frontier = create_pqueue();

  // explored could be a set, it only needs the 
  // test function to work, but for time purposes I'll
  // use the same data structure already set
  explored = create_pqueue();

  // Allocate memory for node
  node = create_node(problem->initial_state, 0, NULL);

  // Add node to frontier
  insert((void*)node, node->path_cost, frontier);

  while(!is_empty(frontier)) {
    node = (Node*) pop(frontier);

    if(problem->goal_test(node)) {
      memo_solution = solution(node);
      destroy_pqueue(frontier);
      destroy_pqueue(explored);
      return memo_solution;
    }

    // Add node state to explored set
    insert(node->state, 0, explored);
    
    // Generate possible actions for this state
    actions_list = problem->actions(node->state);

    // Expand node generating a child for each action
    for(i = 0; actions_list[i] != NULL ; i++) {
      child = make_child(problem, node, actions_list[i]);

      if(!(is_in_queue(child->state, cmp_state_function, explored) || is_in_queue(child, compare_nodes, frontier))) {
        insert((void*)child, child->path_cost + h(child->state), frontier);
      } else if((in_frontier = (Node*) get((void*)child, compare_nodes, frontier)) && (in_frontier->path_cost > child->path_cost)) {
        // replace node in frontier with child
        remove_pqueue_node((void*)in_frontier, compare_nodes, frontier);
        insert((void*)child, child->path_cost, frontier);
      }
    }
  }

  destroy_pqueue(frontier);
  destroy_pqueue(explored);

  return NULL;
}

// Make child function
static Node *make_child(Problem *problem, Node *node, Action action) {
  Node *child;

  child = action(node->state);

  child->parent = node;
  child->path_cost += node->path_cost;

  return child;
}

