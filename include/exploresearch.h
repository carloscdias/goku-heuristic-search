#ifndef _EXPLORE_SEARCH_H_
#define _EXPLORE_SEARCH_H_

#include <utils.h>

byte explored_the_map(State);
double explore_map_heuristic(State);
int get_number_unexplored_regions(byte[MAP_SIZE][MAP_SIZE]);
void init_explored_map(byte[MAP_SIZE][MAP_SIZE]);
void fill_explored_map(byte[MAP_SIZE][MAP_SIZE], Position2D*);

#endif
