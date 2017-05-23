#ifndef _EXPLORE_SEARCH_H_
#define _EXPLORE_SEARCH_H_

#include <utils.h>

#define EXPLORE_ERROR       400

// State definition for the explore problem
typedef struct {
  byte explored_map[MAP_SIZE][MAP_SIZE];
  Position2D current_position;
} ExploreState;

// Main function for this module
void explore_search();

// Goal test function
byte explored_the_map(State);

// Simple heuristic for the problem
double number_of_unexplored_regions(State);

// Compare state function
byte compare_maps(State, State);

// Get path to follow to explore entire map
void *make_explore_path(Node*);

// Movements on the map
Action *movements_on_explore_map(State);

// Actions
Node *exploreUp(const State);
Node *exploreDown(const State);
Node *exploreRight(const State);
Node *exploreLeft(const State);

#endif
