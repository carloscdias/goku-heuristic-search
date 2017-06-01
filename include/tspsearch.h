#ifndef _TSP_SEARCH_H_
#define _TSP_SEARCH_H_

#include <pathsearch.h>

/*
 * TSP problem
 */

// TSP state for this problem
typedef struct {
  position2d_t *position;
  m_stack_t places_to_visit;
} tsp_state_t;

// Function to push the next movements of the agent
void *tsp_make_path(node_t*);

// Main function for this module
void tsp_search(m_stack_t, position2d_t*);

// MST heuristic for this problem
double tsp_heuristic(problem_t*, state_t);

// Goal test function for this module
byte_t tsp_visited_all_places(problem_t*, state_t);

// Movements on the map
action_t *tsp_movements_on_map(state_t);

// Actions
node_t *tsp_move_up(const state_t);
node_t *tsp_move_down(const state_t);
node_t *tsp_move_right(const state_t);
node_t *tsp_move_left(const state_t);

#endif
