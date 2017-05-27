#include <stdlib.h>
#include <utils.h>

// Returns a value limited by the map range
static byte value_inside_map_range(int);

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

// Create position on heap
Place *create_place(byte x, byte y, byte visited) {
  Place *place;

  place = (Place*) malloc(sizeof(Place));

  place->position.x = x;
  place->position.y = y;
  place->visited = visited;

  return place;
}

// Init unexplored map
void init_explored_map(byte current_map[MAP_SIZE][MAP_SIZE]) {
  byte x, y;

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      current_map[x][y] = NOT_EXPLORED;
    }
  }
}

// Fill explored map based on position
void fill_explored_map(byte current_map[MAP_SIZE][MAP_SIZE], Position2D *position) {
  int x, y, startX, startY, endX, endY;

  startX = value_inside_map_range(position->x - DRAGON_RADAR_DISTANCE);
  endX = value_inside_map_range(position->x + DRAGON_RADAR_DISTANCE);

  startY = value_inside_map_range(position->y - DRAGON_RADAR_DISTANCE);
  endY = value_inside_map_range(position->y + DRAGON_RADAR_DISTANCE);

  for(x = startX; x <= endX; x++) {
    for(y = startY; y <= endY; y++) {
      current_map[x][y] = EXPLORED;
    }
  }
}

// Returns a value limited by the map range
static byte value_inside_map_range(int value) {
  if(value < 0) {
    return 0;
  }

  if(value >= MAP_SIZE) {
    return (MAP_SIZE - 1);
  } 

  return value;
}

// Copy maps from src to dst
void copy_explored_map(byte dst[MAP_SIZE][MAP_SIZE], byte src[MAP_SIZE][MAP_SIZE]) {
  byte x, y;

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      dst[x][y] = src[x][y];
    }
  }
}

