#include <stdlib.h>
#include <utils.h>
#include <exploresearch.h>
#include <pathsearch.h>

// TEMPORARIO
#include <stdio.h>

// Explore the map problem
ExploreState *initial_state;

Problem explore_problem = {&initial_state, explored_the_map, movements_on_explore_map};

// Actions
Action explore_general_node[5] = {exploreUp, exploreDown, exploreLeft, exploreRight, NULL};
Action explore_low_x_node[4] = {exploreUp, exploreDown, exploreRight, NULL};
Action explore_low_y_node[4] = {exploreUp, exploreLeft, exploreRight, NULL};
Action explore_high_x_node[4] = {exploreUp, exploreDown, exploreLeft, NULL};
Action explore_high_y_node[4] = {exploreDown, exploreLeft, exploreRight, NULL};
Action explore_corner_top_left_node[3] = {exploreDown, exploreRight, NULL};
Action explore_corner_top_right_node[3] = {exploreDown, exploreLeft, NULL};
Action explore_corner_bottom_left_node[3] = {moveUp, exploreRight, NULL};
Action explore_corner_bottom_right_node[3] = {exploreUp, exploreLeft, NULL};


// Print position funtion for debug purposes
void print_position(Position2D *position) {
  printf("Position: (%d, %d)\n", position->x, position->y);
}

void print_map(ExploreState *state) {
  byte x, y;

  printf("Printing map...\n");

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      printf("%d", state->explored_map[x][y]);
    }
    printf("\n");
  }
}

// Push path to the stack of movements
void *make_explore_path(Node *solution) {
  Position2D *position;

  if(!ll_is_empty(movements)) {
    st_destroy_stack(movements);
  }

  movements = st_create_stack();

  while(solution != NULL) {
    position = create_position(((ExploreState*)solution->state)->current_position.x, ((ExploreState*)solution->state)->current_position.y);
    st_push((void*)position, movements);
    solution = solution->parent;
  }

  return movements;
}

// Explore map search
void explore_search() {

  initial_state = (ExploreState*) malloc(sizeof(ExploreState));

  // Copy current explored map to initial problem state
  copy_explored_map(initial_state->explored_map, EXPLORED_MAP);

  // Copy current Goku position
  initial_state->current_position.x = goku.current_position.x;
  initial_state->current_position.y = goku.current_position.y;

  explore_problem.initial_state = initial_state;

  A_star_search(&explore_problem, number_of_unexplored_regions, compare_maps, make_explore_path);
}

// Count the number of unexplored regions
double number_of_unexplored_regions(State state) {
  double unexplored_regions = 0.0;
  byte x, y;

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      if(((ExploreState*)state)->explored_map[x][y] == 0) {
        unexplored_regions += 1.0;
      }
    }
  }

  return unexplored_regions;
}

// Goal for the explore the map
byte explored_the_map(State state) {
  print_map((ExploreState*)state);
  return (number_of_unexplored_regions(state) <= EXPLORE_ERROR);
}

// Compare maps
byte compare_maps(State state1, State state2) {
  byte x, y;
  unsigned int errors = 0;

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      if(((ExploreState*)state1)->explored_map[x][y] != ((ExploreState*)state2)->explored_map[x][y]) {
        errors++;
      }
    }
  }

  return (errors == 0);
}

// Actions
Node * exploreUp(const State state) {
  ExploreState *es, *new_es;

  es = (ExploreState*) state;
  new_es = (ExploreState*) malloc(sizeof(ExploreState));

  new_es->current_position.x = es->current_position.x;
  new_es->current_position.y = es->current_position.y + 1;

  copy_explored_map(new_es->explored_map, es->explored_map);
  fill_explored_map(new_es->explored_map, &new_es->current_position);

  return create_node((State)new_es, movement_cost(&new_es->current_position), NULL);
}

Node * exploreDown(const State state) {
  ExploreState *es, *new_es;

  es = (ExploreState*) state;
  new_es = (ExploreState*) malloc(sizeof(ExploreState));

  new_es->current_position.x = es->current_position.x;
  new_es->current_position.y = es->current_position.y - 1;

  copy_explored_map(new_es->explored_map, es->explored_map);
  fill_explored_map(new_es->explored_map, &new_es->current_position);

  return create_node((State)new_es, movement_cost(&new_es->current_position), NULL);
}

Node * exploreLeft(const State state) {
  ExploreState *es, *new_es;

  es = (ExploreState*) state;
  new_es = (ExploreState*) malloc(sizeof(ExploreState));

  new_es->current_position.x = es->current_position.x - 1;
  new_es->current_position.y = es->current_position.y;

  copy_explored_map(new_es->explored_map, es->explored_map);
  fill_explored_map(new_es->explored_map, &new_es->current_position);

  return create_node((State)new_es, movement_cost(&new_es->current_position), NULL);
}

Node * exploreRight(const State state) {
  ExploreState *es, *new_es;

  es = (ExploreState*) state;
  new_es = (ExploreState*) malloc(sizeof(ExploreState));

  new_es->current_position.x = es->current_position.x + 1;
  new_es->current_position.y = es->current_position.y;

  copy_explored_map(new_es->explored_map, es->explored_map);
  fill_explored_map(new_es->explored_map, &new_es->current_position);

  return create_node((State)new_es, movement_cost(&new_es->current_position), NULL);
}

// Get possible actions
Action *movements_on_explore_map(State state) {
  Position2D position;

  position.x = ((ExploreState*) state)->current_position.x;
  position.y = ((ExploreState*) state)->current_position.y;

  if(position.x == 0) {

    if(position.y == 0) {
      return explore_corner_bottom_left_node;
    }

    if(position.y == (MAP_SIZE - 1)) {
      return explore_corner_top_left_node;
    }

    return explore_low_x_node;
  }

  if (position.x == (MAP_SIZE - 1)) {

    if(position.y == 0) {
      return explore_corner_bottom_right_node;
    }

    if(position.y == (MAP_SIZE - 1)) {
      return explore_corner_top_right_node;
    }

    return explore_high_x_node;
  }

  if(position.y == (MAP_SIZE - 1)) {
    return explore_high_y_node;
  }

  if(position.y == 0) {
    return explore_low_y_node;
  }

  return explore_general_node;
}

