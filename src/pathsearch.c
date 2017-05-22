#include <stdlib.h>
#include <utils.h>
#include <pathsearch.h>

// Path finding problem
Position2D *initial_state;
Position2D *goal_state;

Problem gokuProblem = {&initial_state, reached_destination, movements_on_map};

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

Stack path_search(byte x1, byte y1, byte x2, byte y2) {
  initial_state = create_position(x1, y1);
  goal_state = create_position(x2, y2);

  gokuProblem.initial_state = initial_state;

  return (Stack)A_star_search(&gokuProblem, manhatan_distance_to_destination, compare_positions, make_path);
}

// Get possible actions
Action *movements_on_map(State state) {
  Position2D *position;
  position = (Position2D*) state;

  if(position->x == 0) {

    if(position->y == 0) {
      return corner_bottom_left_node;
    }

    if(position->y == (MAP_SIZE - 1)) {
      return corner_top_left_node;
    }

    return low_x_node;
  }

  if (position->x == (MAP_SIZE - 1)) {

    if(position->y == 0) {
      return corner_bottom_right_node;
    }

    if(position->y == (MAP_SIZE - 1)) {
      return corner_top_right_node;
    }

    return high_x_node;
  }

  if(position->y == (MAP_SIZE - 1)) {
    return high_y_node;
  }

  if(position->y == 0) {
    return low_y_node;
  }

  return general_node;
}

// Actions
Node * moveUp(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x, position->y + 1);

  return create_node((State)new_position, movement_cost(new_position), NULL);
}

Node * moveDown(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x, position->y - 1);

  return create_node((State)new_position, movement_cost(new_position), NULL);
}

Node * moveLeft(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x - 1, position->y);

  return create_node((State)new_position, movement_cost(new_position), NULL);
}

Node * moveRight(const State state) {
  Position2D *position, *new_position;
  position = (Position2D*) state;

  new_position = create_position(position->x + 1, position->y);

  return create_node((State)new_position, movement_cost(new_position), NULL);
}

// Goal test function
byte reached_destination(State state) {
  return compare_positions((Position2D*) state, goal_state);
}

// Heuristic function for this problem
double manhatan_distance_to_destination(State state) {
  Position2D *position;
  position = (Position2D*) state;
  return (double)(abs(position->x - goal_state->x) + abs(position->y - goal_state->y));
}

