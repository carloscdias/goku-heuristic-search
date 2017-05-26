#ifndef _EXPLORE_SEARCH_H_
#define _EXPLORE_SEARCH_H_

// Main function for this module
void explore_search();

// Get the path cost/explore rate
double path_cost_explore_rate(byte, byte, byte, byte);

// Get the number of explored regions in a new map
unsigned int count_explored_regions(byte[MAP_SIZE][MAP_SIZE], const byte[MAP_SIZE][MAP_SIZE]);

// Unitary search for parallel computing
void *parallel_searching(void*);

#endif
