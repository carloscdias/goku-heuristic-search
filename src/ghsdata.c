#include <stdlib.h>
#include <ghsdata.h>

// Cost of the movement on the map
byte_t
movement_cost (position2d_t *position)
{
  char current_state = game.map[position->x][position->y];

  switch (current_state)
  {
    case 'G': return GRASS_COST;
    case 'A': return WATER_COST;
    case 'M': return MOUNTAIN_COST;
  }

  // return max value 
  return -1;
}

// Absolute value for integers
int
abs (int value)
{
  return ((value < 0)?-1*value:value);
}

// Simple compare function for points in the 2d space
byte_t
compare_positions (void *state1, void *state2)
{
  position2d_t *position1, *position2;
  position1 = (position2d_t*) state1;
  position2 = (position2d_t*) state2;

  return ((position1->x == position2->x) && (position1->y == position2->y));
}

// Create position on heap
position2d_t
*create_position (byte_t x, byte_t y)
{
  position2d_t *position;

  position = (position2d_t*) malloc(sizeof(position2d_t));

  position->x = x;
  position->y = y;

  return position;
}

// Create place on heap
place_t
*create_place (byte_t x, byte_t y, byte_t visited)
{
  place_t *place;

  place = (place_t*) malloc(sizeof(place_t));

  place->position.x = x;
  place->position.y = y;
  place->visited = visited;

  return place;
}

// Init unexplored map
void
init_explored_map (byte_t current_map[MAP_SIZE][MAP_SIZE])
{
  byte_t x, y;

  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      current_map[x][y] = NOT_EXPLORED;
    }
  }
}

// Fill explored map based on position
void
fill_explored_map (byte_t current_map[MAP_SIZE][MAP_SIZE], position2d_t *position)
{
  int x, y, startX, startY, endX, endY;

  startX = value_inside_map_range (position->x - DRAGON_RADAR_DISTANCE);
  endX = value_inside_map_range (position->x + DRAGON_RADAR_DISTANCE);

  startY = value_inside_map_range (position->y - DRAGON_RADAR_DISTANCE);
  endY = value_inside_map_range (position->y + DRAGON_RADAR_DISTANCE);

  for (x = startX; x <= endX; x++) {
    for (y = startY; y <= endY; y++) {
      current_map[x][y] = EXPLORED;
    }
  }
}

// Returns a value limited by the map range
byte_t
value_inside_map_range (int value)
{
  if (value < 0) {
    return 0;
  }

  if (value >= MAP_SIZE) {
    return (MAP_SIZE - 1);
  } 

  return value;
}

// Copy maps from src to dst
void
copy_explored_map (byte_t dst[MAP_SIZE][MAP_SIZE], byte_t src[MAP_SIZE][MAP_SIZE])
{
  byte_t x, y;

  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      dst[x][y] = src[x][y];
    }
  }
}

// Helper to toggle a value
void toggle(byte_t *value) {
  if (*value == 0) {
    *value = 1;
  } else {
    *value = 0;
  }
}

