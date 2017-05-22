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
byte reached_destination(Node*);

// Heuristic for this module
double manhatan_distance_to_destination(Node*);

// Main search for this module
Stack path_search(byte, byte, byte, byte);

#endif
