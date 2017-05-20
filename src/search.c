#include <stdlib.h>
#include <search.h>
#include <pqueue.h>

// Create node
Node *create_node(State state, double cost, Node* parent) {
  Node *node;

  node = (Node *) malloc(sizeof(Node));

  node->state = state;
  node->path_cost = cost;
  node->parent = parent;

  return node;
}

// Global to store compare function
// based in state received in search
byte (*_cmp_function)(void*, void*);

// A function to compare states receiving nodes
byte compare_nodes(void *node1, void *node2) {
  return _cmp_function(((Node*)node1)->state, ((Node*)node2)->state);
}

// A Star algorithm
Node *A_star_search(Problem *problem, double (*h)(State), byte (*cmp_state_function)(void*, void*) ){
  Node *node, *child, *in_frontier;
  PQueue *frontier, *explored;
  ActionsList *actions_list;
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

    if(problem->goal_test(node->state)) {
      destroy_pqueue(frontier);
      destroy_pqueue(explored);
      return node;
    }

    // Add node state to explored set
    insert(node->state, 0, explored);
    
    // Generate possible actions for this state
    actions_list = problem->actions(node->state);

    // Expand node generating a child for each action
    for(i = 0; i < actions_list->number_of_actions; i++) {
      child = make_child(problem, node, actions_list->actions[i], h);

      if(!(is_in_queue(child->state, cmp_state_function, explored) || is_in_queue(child, _cmp_function, frontier))) {
        insert((void*)child, child->path_cost, frontier);
      } else if((in_frontier = (Node*) get((void*)child, _cmp_function, frontier)) && (in_frontier->path_cost > child->path_cost)) {
        // replace node in frontier with child
        remove_pqueue_node((void*)in_frontier, _cmp_function, frontier);
        insert((void*)child, child->path_cost, frontier);
      }
    }
  }

  destroy_pqueue(frontier);
  destroy_pqueue(explored);

  return NULL;
}

// Make child function
// g(s) is a function which returns the cost 
static Node *make_child(Problem *problem, Node *node, Action action, double (*h)(State)) {
  Node *child;

  child = action(node->state);

  child->parent = node;

  child->path_cost += h(child->state);

  return child;
}

