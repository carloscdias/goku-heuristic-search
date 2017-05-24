#ifndef _PATH_SEARCH_H_
#define _PATH_SEARCH_H_

#include <utils.h>

// Goku movements as actions
Node * moveUp(const State);
Node * moveDown(const State);
Node * moveLeft(const State);
Node * moveRight(const State);

// Possible list of movements according to a given state
Action *movements_on_map(State);

// Goal test for this module
byte reached_destination(State);

// Heuristic for this module
double manhatan_distance_to_destination(State);

// Main search for this module
void path_search(byte, byte, byte, byte);

// Search to get the best path cost only
unsigned int path_cost(byte, byte, byte, byte);

// Create solution as stack and put it on movements
void *make_path(Node*);

#endif
