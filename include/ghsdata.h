#ifndef _GHSDATA_H_
#define _GHSDATA_H_

#include <stack.h>

// Problem constraints
#define MAP_SIZE              42
#define DRAGONBALLS_NUMBER    7
#define DRAGON_RADAR_DISTANCE 3
#define EXPLORED              1
#define NOT_EXPLORED          0
#define GRASS_COST            1
#define WATER_COST            10
#define MOUNTAIN_COST         60

// Simple data structures used accross the program
typedef unsigned char byte_t;

// Position in a 2D world
typedef struct {
  byte_t x, y;
} position2d_t;

// Structure for agent
typedef struct {
  position2d_t current_position;
  position2d_t last_position;
} agent_t;

// Dragonball structure
typedef struct {
  byte_t x, y, seen, caught;
} dragonball_t;

// Searching status
typedef enum {IDLE = 0, START} status_t;

// Game structure
typedef struct {
  // Visual options
  byte_t show_grid, show_dragonballs, show_info, show_radar;

  // Current number of caught dragonballs
  byte_t caught_dragonballs;

  // Search status
  status_t status;

  // Current cost
  unsigned int current_total_cost;

  // Agent
  agent_t goku;

  // Dragonballs
  dragonball_t dragonballs[DRAGONBALLS_NUMBER];

  // Map data
  byte_t map[MAP_SIZE][MAP_SIZE];
  byte_t explored_map[MAP_SIZE][MAP_SIZE];

  // Stack for next movements to be performed
  m_stack_t movements;

  // Stacks for searching dragonballs
  // Just seen dragonballs are here
  m_stack_t seen_dragonballs;
  // Now searching dragonbals are placed here
  m_stack_t tracked_dragonballs;
} game_t;

// Global variable with game data
extern game_t game;

// Util functions for the problems
byte_t movement_cost(position2d_t*);

// Integer absolute value
int abs(int);

// Compare two positions, receiving generic pointers
// as parameters for search purposes
byte_t compare_positions(void*, void*);

// Create 2D position on heap
position2d_t *create_position(byte_t, byte_t);

// Returns a value limited by the map range
byte_t value_inside_map_range(int);

// Init map of explored region with NOT_EXPLORED constant
void init_explored_map(byte_t[MAP_SIZE][MAP_SIZE]);

// Fill map with EXPLORED constant at given position around Dragonradar
void fill_explored_map(byte_t[MAP_SIZE][MAP_SIZE], position2d_t*);

// Copy explored map
void copy_explored_map(byte_t[MAP_SIZE][MAP_SIZE], byte_t[MAP_SIZE][MAP_SIZE]);

// Toggle byte
void toggle(byte_t*);

#endif
