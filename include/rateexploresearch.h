#ifndef _RATE_EXPLORE_SEARCH_H_
#define _RATE_EXPLORE_SEARCH_H_

#include <ghsdata.h>

// Main function for this module
void res_explore_search();

// Get the path cost/explore rate
double res_path_cost_explore_rate(byte_t, byte_t, byte_t, byte_t);

// Get the number of explored regions in a new map
unsigned int res_count_explored_regions(byte_t[MAP_SIZE][MAP_SIZE], const byte_t[MAP_SIZE][MAP_SIZE]);

// Unitary search for parallel computing
void *res_parallel_searching(void*);

#endif
