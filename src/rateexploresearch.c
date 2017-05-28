#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <pthread.h>
#include <ghsdata.h>
#include <pathsearch.h>
#include <rateexploresearch.h>

// Rate map
double RATE_MAP[MAP_SIZE][MAP_SIZE];

// Get rate of cost/explored_area in a path solution
static void *res_get_cost_explore_rate(node_t*);

// Unitary search function to parallel computing
void
*res_parallel_searching (void *param)
{
  position2d_t *position;

  position = (position2d_t*) param;

  if (game.map[position->x][position->y] == 'G') {
    RATE_MAP[position->x][position->y] = res_path_cost_explore_rate (game.goku.current_position.x, game.goku.current_position.y, position->x, position->y);
  } else {
    RATE_MAP[position->x][position->y] = DBL_MAX;
  }


  free (position);

  return NULL;
}

// Explore map search
void
res_explore_search()
{
  position2d_t next_place, *position;
  pthread_t tid[MAP_SIZE][MAP_SIZE];
  byte_t x, y;
  double current_best;

  // pthread this shit!
  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      position = create_position (x, y);
      pthread_create (&tid[x][y], NULL, res_parallel_searching, position);
    }
  }

  // Wait all threads finish processing 
  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      pthread_join (tid[x][y], NULL);
    }
  }

  current_best = DBL_MAX;

  // Get the min value in matrix
  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      if (RATE_MAP[x][y] < current_best) {
        current_best = RATE_MAP[x][y];
        next_place.x = x;
        next_place.y = y;
      }
    }
  }

  // Do the movement
  ps_path_search (game.goku.current_position.x, game.goku.current_position.y, next_place.x, next_place.y);
}

// Get the rate explore/cost
void
*res_get_cost_explore_rate (node_t *solution)
{
  byte_t map[MAP_SIZE][MAP_SIZE];
  double *cost;
  unsigned int explored_region;

  cost = malloc(sizeof(double));
  *cost = solution->path_cost;

  init_explored_map (map);

  while (solution != NULL) {
    fill_explored_map (map, (position2d_t*) solution->state);
    solution = solution->parent;
  }

  explored_region = res_count_explored_regions (map, game.explored_map);

  if (explored_region == 0) {
    *cost = DBL_MAX;
  } else {
    *cost = *cost / explored_region;
  }

  return ((void*) cost);
}

// Returns only the path cost/explore rate
double
res_path_cost_explore_rate (byte_t x1, byte_t y1, byte_t x2, byte_t y2)
{
  double cost, *tmp;

  tmp = (double*) ps_generic_path_search (x1, y1, x2, y2, res_get_cost_explore_rate);
  cost = *tmp;
  free (tmp);

  return cost;
}

// Count number of explored regions
unsigned int
res_count_explored_regions (byte_t current_map[MAP_SIZE][MAP_SIZE], const byte_t explored_map[MAP_SIZE][MAP_SIZE])
{
  byte_t x, y;
  unsigned int explored_regions = 0;

  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      if ((current_map[x][y] == EXPLORED) && (explored_map[x][y] == NOT_EXPLORED)) {
        explored_regions++;
      }
    }
  }

  return explored_regions;
}

