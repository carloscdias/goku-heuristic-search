#ifndef _GOKU_UTILS_H_
#define _GOKU_UTILS_H_

#include <stack.h>
#include <search.h>

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
typedef unsigned char byte;

typedef struct {
  byte x, y;
} Position2D;

// Map
extern byte MAP[MAP_SIZE][MAP_SIZE];
extern byte EXPLORED_MAP[MAP_SIZE][MAP_SIZE];

// Problems to be solved
extern Problem gokuProblem;
extern Problem mapExploreProblem;

// Path to be followed
extern Stack movements;

// Entity variables
extern Position2D Goku;
extern Position2D Dragonballs[DRAGONBALLS_NUMBER];

// Util functions for the problems
byte movement_cost(Position2D*);

// Integer absolute value
int abs(int);

// Compare two positions, receiving generic pointers
// as parameters for search purposes
byte compare_positions(void*, void*);

// Create 2D position on heap
Position2D *create_position(byte, byte);

// Init map of explored region with NOT_EXPLORED constant
void init_explored_map(byte[MAP_SIZE][MAP_SIZE]);

// Fill map with EXPLORED constant at given position around Dragonradar
void fill_explored_map(byte[MAP_SIZE][MAP_SIZE], Position2D*);

// Copy explored map
void copy_explored_map(byte[MAP_SIZE][MAP_SIZE], byte[MAP_SIZE][MAP_SIZE]);

#endif
