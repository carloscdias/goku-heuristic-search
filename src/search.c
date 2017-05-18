#include <search.h>

// A Star algorithm
Path A_star_search(Problem *problem) {
  Node *node;

  // Allocate memory for node
  node->state = problem->initial_state;
  node->path_cost = 0;
  node->parent = NULL;

  frontier = NULL;
  explored = NULL;
}

