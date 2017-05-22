#include <stdlib.h>
#include <utils.h>

// Cost of the movement on the map
byte movement_cost(Position2D *position) {
  char current_state = MAP[position->x][position->y];

  switch(current_state) {
    case 'G': return GRASS_COST;
    case 'A': return WATER_COST;
    case 'M': return MOUNTAIN_COST;
  }

  // return max value 
  return -1;
}

// Absolute value for integers
int abs(int value) {
  return ((value < 0)?-1*value:value);
}

// Simple compare function for points in the 2d space
byte compare_positions(void *state1, void *state2) {
  Position2D *position1, *position2;
  position1 = (Position2D*) state1;
  position2 = (Position2D*) state2;

  return ((position1->x == position2->x) && (position1->y == position2->y));
}

// Create position on heap
Position2D *create_position(byte x, byte y) {
  Position2D *position;

  position = (Position2D*) malloc(sizeof(Position2D));

  position->x = x;
  position->y = y;

  return position;
}

