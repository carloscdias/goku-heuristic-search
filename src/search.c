#include <search.h>

// A Star algorithm
Path A_star_search(Problem *problem) {
  Node *node, *child, *in_frontier;
  ActionsList *actions_list;
  unsigned int i;

  // Allocate memory for node
  node->state = problem->initial_state;
  node->path_cost = 0;
  node->parent = NULL;

  // Add node to frontier
  frontier = NULL;
  explored = NULL;

  while(!is_empty(frontier)) {
    node = pop(frontier);

    if(problem->goal_test(node->state)) {
      return solution(node);
    }

    add(node->state, explored);
    
    // Generate possible actions for this state
    actions_list = problem->actions(node->state);

    // Expand node generating a child for each action
    for(i = 0; i < actions_list->number_of_actions; i++) {
      child = make_child(problem, node, actions_list->actions[i]);

      if(!(is_in(child->state, explored) || is_in(child->state, frontier))) {
        insert(child, frontier);
      } else if((in_frontier = get(child->state, frontier)) && (in_frontier->path_cost > child->path_cost)) {
        // replace node in frontier with child
      }
    }
  }

  return NULL;
}

