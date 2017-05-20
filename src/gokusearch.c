#include <gokusearch.h>
#include <types.h>

// Problem definition
Problem gokuProblem;

// Goal test function
byte got_the_dragonballs(State state) {
  Position2D *position;
  position = (Position2D*) state;
  return ((position->x == 25) && (position->y == 30));
}

// Get possible actions
ActionsList *movements_on_map(State state) {
  Position2D *position;
  position = (Position2D*) state;

  if((position->x == 0) && (position->y == 0)) {
  }
}

