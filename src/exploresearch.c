#include <stdlib.h>
#include <utils.h>
#include <pathsearch.h>
#include <exploresearch.h>
#include <float.h>
#include <pthread.h>
#include <stdio.h>

// Rate map
double RATE_MAP[MAP_SIZE][MAP_SIZE];

// Print position funtion for debug purposes
void print_position(Position2D *position) {
  printf("Position: (%d, %d)\n", position->x, position->y);
}

// Get rate of cost/explored_area in a path solution
static void *_get_cost_explore_rate(Node*);

// Unitary search function to parallel computing
void *parallel_searching(void *param) {
  Position2D *position;

  position = (Position2D*)param;

  RATE_MAP[position->x][position->y] = path_cost_explore_rate(goku.current_position.x, goku.current_position.y, position->x, position->y);

  free(position);

  return NULL;
}

// Explore map search
void explore_search() {
  Position2D next_place, *position;
  pthread_t tid[MAP_SIZE][MAP_SIZE];
  byte x, y;
  double current_best;

  // pthread this shit!
  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      position = create_position(x, y);
      pthread_create(&tid[x][y], NULL, parallel_searching, position);
    }
  }

  // Wait all threads finish processing 
  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      pthread_join(tid[x][y], NULL);
    }
  }

  current_best = DBL_MAX;

  // Get the min value in matrix
  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      if(RATE_MAP[x][y] < current_best) {
        current_best = RATE_MAP[x][y];
        next_place.x = x;
        next_place.y = y;
      }
    }
  }

  // Do the movement
  path_search(goku.current_position.x, goku.current_position.y, next_place.x, next_place.y);
}

// Get the rate explore/cost
void *_get_cost_explore_rate(Node *solution) {
  byte map[MAP_SIZE][MAP_SIZE];
  double *cost;
  unsigned int explored_region;

  cost = malloc(sizeof(double));
  *cost = solution->path_cost;

  init_explored_map(map);

  while(solution != NULL) {
    fill_explored_map(map, (Position2D*)solution->state);
    solution = solution->parent;
  }

  explored_region = count_explored_regions(map, EXPLORED_MAP);

  if (explored_region == 0) {
    *cost = DBL_MAX;
  } else {
    *cost = *cost / explored_region;
  }

  return ((void*)cost);
}

// Returns only the path cost/explore rate
double path_cost_explore_rate(byte x1, byte y1, byte x2, byte y2) {
  double cost, *tmp;

  tmp = (double*) _path_search(x1, y1, x2, y2, _get_cost_explore_rate);
  cost = *tmp;
  free(tmp);

  return cost;
}

// Count number of explored regions
unsigned int count_explored_regions(byte current_map[MAP_SIZE][MAP_SIZE], const byte explored_map[MAP_SIZE][MAP_SIZE]) {
  byte x, y;
  unsigned int explored_regions = 0;

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      if((current_map[x][y] == EXPLORED) && (explored_map[x][y] == NOT_EXPLORED)) {
        explored_regions++;
      }
    }
  }

  return explored_regions;
}

