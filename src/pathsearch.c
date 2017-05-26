#include <stdlib.h>
#include <utils.h>
#include <pathsearch.h>

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

// Push solution to the movements stack
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

// Main search function for this module
void *_path_search(byte x1, byte y1, byte x2, byte y2, void *(*solution)(Node*)) {
  Problem path_problem;
  void *data; 

  path_problem.initial_state = create_position(x1, y1);
  path_problem.final_state = create_position(x2, y2);
  path_problem.goal_test = reached_destination;
  path_problem.actions = movements_on_map;

  data = A_star_search(&path_problem, manhatan_distance_to_destination, compare_positions, solution);

  free(path_problem.final_state);

  return data;
}

// Path search push to the stack of movements the best path between two points
void path_search(byte x1, byte y1, byte x2, byte y2) {
  _path_search(x1, y1, x2, y2, make_path);
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

// Goal test for this module
byte reached_destination(Problem *problem, State state) {
  return compare_positions((Position2D*)problem->final_state, (Position2D*)state);
}

// Heuristic function for this problem
double manhatan_distance_to_destination(Problem *problem, State state) {
  Position2D *position, *destination;
  position = (Position2D*) state;
  destination = (Position2D*) problem->final_state;
  return (double)(abs(position->x - destination->x) + abs(position->y - destination->y));
}

