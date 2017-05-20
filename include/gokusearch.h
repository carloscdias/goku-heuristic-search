#ifndef _GOKUSEARCH_H_
#define _GOKUSEARCH_H_

#include <types.h>
#include <search.h>

#define MAP_SIZE              42
#define DRAGONBALLS_NUMBER    7
#define DRAGON_RADAR_DISTANCE 3

#define GRASS_COST            1
#define WATER_COST            10
#define MOUNTAIN_COST         60

// Here are the functions which the
// a-star algorithm needs to run properly

extern byte MAP[MAP_SIZE][MAP_SIZE];

extern Problem gokuProblem;

Action *movements_on_map(State);
Position2D *create_position(byte, byte);
double cost(Position2D*);
Node * moveUp(const State);
Node * moveDown(const State);
Node * moveLeft(const State);
Node * moveRight(const State);
int abs(int);
byte got_the_dragonballs(State);
double heuristic(State);
byte compare_positions(void*, void*);
void *goku_search(byte, byte, byte, byte);

#endif
