#ifndef _PATH_SEARCH_H_
#define _PATH_SEARCH_H_

#include <ghsdata.h>
#include <astar.h>

// Goku movements as actions
node_t *ps_move_up(const state_t);
node_t *ps_move_down(const state_t);
node_t *ps_move_left(const state_t);
node_t *ps_move_right(const state_t);

// Possible list of movements according to a given state
action_t *ps_movements_on_map(state_t);

// Goal test for this module
byte_t ps_reached_destination(problem_t*, state_t);

// Heuristic for this module
double ps_manhatan_distance_to_destination(problem_t*, state_t);

// Main search for this module
void ps_path_search(byte_t, byte_t, byte_t, byte_t);

// Search to get the best path cost only
unsigned int ps_path_cost(byte_t, byte_t, byte_t, byte_t);

// Create solution as stack and put it on movements
void *ps_make_path(node_t*);

// Generic search best path function
void *ps_generic_path_search(byte_t, byte_t, byte_t, byte_t, void *(*)(node_t*));

#endif
