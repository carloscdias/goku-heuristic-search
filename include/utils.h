#ifndef _GOKU_UTILS_H_
#define _GOKU_UTILS_H_

#include <stack.h>
#include <search.h>

// Problem constraints
#define MAP_SIZE              42
#define DRAGONBALLS_NUMBER    7
#define DRAGON_RADAR_DISTANCE 3
#define GRASS_COST            1
#define WATER_COST            10
#define MOUNTAIN_COST         60

// Map
extern byte MAP[MAP_SIZE][MAP_SIZE];

// Problems to be solved
extern Problem gokuProblem;
extern Problem mapExploreProblem;

// Path to be followed
extern Stack movements;

// Entity variables
extern Position2D Goku;
extern Position2D Dragonballs[DRAGONBALLS_NUMBER];

// Simple data structures used accross the program
typedef unsigned char byte;

typedef struct {
  byte x, y;
} Position2D;

// Util functions for the problems
byte movement_cost(Position2D*);

// Integer absolute value
int abs(int);

// Compare two positions, receiving generic pointers
// as parameters for search purposes
byte compare_positions(void*, void*);

// Create 2D position on heap
Position2D *create_position(byte, byte);

#endif
