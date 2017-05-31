#ifndef _TSP_SEARCH_H_
#define _TSP_SEARCH_H_

#include <pathsearch.h>

/*
 * TSP problem
 */

typedef struct {
  position2d_t *position;
  m_stack_t places_to_visit;
} tsp_state_t;


void tsp_make_path(node_t*);

void tsp_search(m_stack_t, position2d_t*);

double tsp_heuristic(problem_t*, state_t);

byte_t visited_all_places(problem_t*, state_t);

action_t *tsp_movements_on_map(state_t);

node_t *tsp_move_up(const state);

node_t *tsp_move_down(const state);

node_t *tsp_move_right(const state);

node_t *tsp_move_left(const state);

#endif
