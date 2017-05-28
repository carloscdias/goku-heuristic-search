#include <astar.h>
#include <stdlib.h>
#include <pqueue.h>

// Static function that generates a child node
static node_t *as_make_child (problem_t*, node_t*, action_t);

// Static function to compare nodes based on node state
static unsigned char as_compare_nodes(void*, void*);

// Global to store compare function
// based in state received in search
unsigned char (*as_cmp_function)(void*, void*);

// Create node
node_t
*as_create_node (state_t state, double cost, node_t* parent)
{
  node_t *node;

  node = (node_t*) malloc(sizeof(node_t));

  node->state = state;
  node->path_cost = cost;
  node->parent = parent;

  return node;
}

// A function to compare states receiving nodes
unsigned char
as_compare_nodes (void *node1, void *node2)
{
  return as_cmp_function(((node_t*) node1)->state, ((node_t*) node2)->state);
}

// A Star algorithm
void *
as_a_star_search (problem_t *problem, double (*h)(problem_t*, state_t),
    unsigned char (*cmp_state_function)(void*, void*), void *(*solution)(node_t*))
{
  node_t *node, *child, *in_frontier;
  pqueue_t *frontier, *explored;
  action_t *actions_list;
  void *memo_solution;
  unsigned int i;

  as_cmp_function = cmp_state_function;

  // priority queue data structure
  frontier = pq_create_pqueue();

  // explored could be a set, it only needs the 
  // test function to work, but for time purposes I'll
  // use the same data structure already set
  explored = pq_create_pqueue();

  // Allocate memory for node
  node = as_create_node (problem->initial_state, 0, NULL);

  // Add node to frontier
  pq_insert ((void*)node, node->path_cost, frontier);

  while (!pq_is_empty (frontier)) {
    node = (node_t*) pq_pop (frontier);

    if (problem->goal_test (problem, node->state)) {
      memo_solution = solution(node);
      pq_destroy_pqueue(frontier);
      pq_destroy_pqueue(explored);
      return memo_solution;
    }

    // Add node state to explored set
    pq_insert (node->state, 0, explored);
    
    // Generate possible actions for this state
    actions_list = problem->actions (node->state);

    // Expand node generating a child for each action
    for(i = 0; actions_list[i] != NULL ; i++) {
      child = as_make_child (problem, node, actions_list[i]);

      if (!(pq_is_in_queue (child->state, cmp_state_function, explored) || 
            pq_is_in_queue (child, as_compare_nodes, frontier))) {
        pq_insert ((void*)child, child->path_cost + h(problem, child->state), frontier);
      } else if ((in_frontier = (node_t*) pq_get((void*)child, as_compare_nodes, frontier)) &&
          (in_frontier->path_cost > child->path_cost)) {
        // replace node in frontier with child
        pq_remove_pqueue_node ((void*)in_frontier, as_compare_nodes, frontier);
        pq_insert ((void*)child, child->path_cost, frontier);
      }
    }
  }

  pq_destroy_pqueue(frontier);
  pq_destroy_pqueue(explored);

  return NULL;
}

// Make child function
static node_t
*as_make_child (problem_t *problem, node_t *node, action_t action)
{
  node_t *child;

  child = action (node->state);

  child->parent = node;
  child->path_cost += node->path_cost;

  return child;
}

