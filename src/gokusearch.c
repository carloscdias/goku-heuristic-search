#include <stdlib.h>
#include <types.h>
#include <stack.h>
#include <search.h>
#include <gokusearch.h>

// Map allocation in memory
byte MAP[MAP_SIZE][MAP_SIZE];

Stack movements = NULL;

// Problem definition
Position2D * initial_state;
Position2D * goal_state;
Problem gokuProblem = {&initial_state, got_the_dragonballs, movements_on_map};

// Actions
Action general_node[5] = {moveUp, moveDown, moveLeft, moveRight, NULL};
Action low_x_node[4] = {moveUp, moveDown, moveRight, NULL};
Action low_y_node[4] = {moveUp, moveLeft, moveRight, NULL};
Action high_x_node[4] = {moveUp, moveDown, moveLeft, NULL};
Action high_y_node[4] = {moveDown, moveLeft, moveRight, NULL};
Action corner_top_left_node[3] = {moveDown, moveRight, NULL};
Action corner_top_right_node[3] = {moveDown, moveLeft, NULL};
Action corner_bottom_left_node[3] = {moveUp, moveRight, NULL};
Action corner_bottom_right_node[3] = {moveUp, moveLeft, NULL};

void *make_path(Node *solution) {
  Position2D *position;

  if(!ll_is_empty(movements)) {
    st_destroy_stack(movements);
  }

  movements = st_create_stack();

  while(solution != NULL) {
    position = create_position(((Position2D*)solution->state)->x, ((Position2D*)solution->state)->y);
    st_push((void*)position, movements);
    solution = solution->parent;
  }

  return movements;
}

Stack goku_search(byte x1, byte y1, byte x2, byte y2) {
  initial_state = create_position(x1, y1);
  goal_state = create_position(x2, y2);

  gokuProblem.initial_state = initial_state;

  return (Stack)A_star_search(&gokuProblem, heuristic, compare_positions, make_path);
}

// Get possible actions
Action *movements_on_map(State state) {
  Position2D *position;
  position = (Position2D*) state;

  if(position->x == 0) {

    if(position->y == 0) {
      return corner_bottom_left_node;
    }

    if(position->y == MAP_SIZE - 1) {
      return corner_top_left_node;
    }

    return low_x_node;
  }

  if (position->x == MAP_SIZE - 1) {

    if(position->y == 0) {
      return corner_bottom_right_node;
    }

    if(position->y == MAP_SIZE - 1) {
      return corner_top_right_node;
    }

    return high_y_node;
  }

  if(position->y == MAP_SIZE - 1) {
    return high_y_node;
  }

  if(position->y == 0) {
    return low_y_node;
  }

  return general_node;
}

// Create position on heap
Position2D *create_position(byte x, byte y) {
  Position2D *position;

  position = (Position2D*) malloc(sizeof(Position2D));

  position->x = x;
  position->y = y;

  return position;
}

// Calculate cost for new position
double cost(Position2D *position) {
  char current_state = MAP[position->x][position->y];

  switch(current_state) {
    case 'G': return GRASS_COST;
    case 'A': return WATER_COST;
    case 'M': return MOUNTAIN_COST;
  }

  // return max value 
  return MOUNTAIN_COST;
}

// Actions
Node * moveUp(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x, position->y + 1);

  return create_node((State)new_position, cost(new_position), NULL);
}

Node * moveDown(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x, position->y - 1);

  return create_node((State)new_position, cost(new_position), NULL);
}

Node * moveLeft(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x - 1, position->y);

  return create_node((State)new_position, cost(new_position), NULL);
}

Node * moveRight(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x + 1, position->y);

  return create_node((State)new_position, cost(new_position), NULL);
}

int abs(int value) {
  return ((value < 0)?-1*value:value);
}

// Goal test function
byte got_the_dragonballs(State state) {
  return compare_positions((Position2D*) state, goal_state);
}

// Heuristic function for this problem
double heuristic(State state) {
  Position2D *position;
  position = (Position2D*) state;

  // Manhatan distance for example point
  return (abs(position->x - goal_state->x) + abs(position->y - goal_state->y));
}

byte compare_positions(void *state1, void *state2) {
  Position2D *position1, *position2;
  position1 = (Position2D*) state1;
  position2 = (Position2D*) state2;

  // Simple compare function for points in the 2d space
  return ((position1->x == position2->x) && (position1->y == position2->y));
}

