#include <stdlib.h>
#include <tspsearch.h>
#include <stdio.h>

// Actions
action_t tsp_general_node[5]              = {tsp_move_up,   tsp_move_down,
                                             tsp_move_left,  tsp_move_right, NULL};
action_t tsp_low_x_node[4]                = {tsp_move_up,   tsp_move_down,  tsp_move_right, NULL};
action_t tsp_low_y_node[4]                = {tsp_move_up,   tsp_move_left,  tsp_move_right, NULL};
action_t tsp_high_x_node[4]               = {tsp_move_up,   tsp_move_down,  tsp_move_left,  NULL};
action_t tsp_high_y_node[4]               = {tsp_move_down, tsp_move_left,  tsp_move_right, NULL};
action_t tsp_corner_top_left_node[3]      = {tsp_move_down, tsp_move_right, NULL};
action_t tsp_corner_top_right_node[3]     = {tsp_move_down, tsp_move_left,  NULL};
action_t tsp_corner_bottom_left_node[3]   = {tsp_move_up,   tsp_move_right, NULL};
action_t tsp_corner_bottom_right_node[3]  = {tsp_move_up,   tsp_move_left,  NULL};

// helper to create a tsp state
static tsp_state_t
*create_tsp_state(byte_t x, byte_t y)
{
  tsp_state_t *tsp_state;

  tsp_state = (tsp_state_t*) malloc(sizeof(tsp_state_t));

  tsp_state->position = create_position (x, y);
  tsp_state->places_to_visit = st_create_stack();

  return tsp_state;
}

// helper to destroy a tsp state
static void
destroy_tsp_state(tsp_state_t *tsp_state)
{
  if (tsp_state->position != NULL) {
    // Deallocate current position
    free(tsp_state->position);
  }

  // Deallocate stack
  st_destroy_stack(tsp_state->places_to_visit);
  // Deallcate state
  free(tsp_state);
}

// helper to create a stack of unvisited places
static void
create_unvisited_places_stack (m_stack_t old_stack, m_stack_t new_stack, position2d_t *current_position)
{
  linkedlistnode_t *node;
  position2d_t *data, *copy;

  if (ll_is_empty(old_stack)) {
    return;
  }

  // Copy elements of old stack to the new stack
  node = old_stack->first_node;
  while (node != NULL) {
    data = (position2d_t*) node->data;

    // unless the element is being visited right now
    if (compare_positions(data, current_position) == 0) {
      copy = create_position(data->x, data->y);
      st_push(copy, new_stack);
    }

    node = node->next;
  }
}

// Function to compare two tsp_states and returns
// 1 if they are equals or 0 if they are different
static byte_t
compare_tsp_states(void *tsp1, void *tsp2)
{
  tsp_state_t *state1, *state2;
  
  state1 = (tsp_state_t*) tsp1;
  state2 = (tsp_state_t*) tsp2;

  // Here we have to decide how to tsp states will be compared
  return compare_positions(state1->position, state2->position) && state1->places_to_visit->number_of_elements == state2->places_to_visit->number_of_elements;
}

// Push solution to the movements stack
void
*tsp_make_path (node_t *solution)
{
  position2d_t *position;

  if (!ll_is_empty (game.movements)) {
    st_clear_stack (game.movements);
  }
  printf("found a solution for the tsp problem :)\n");

  while (solution != NULL) {
    position = create_position (((tsp_state_t*) solution->state)->position->x, ((tsp_state_t*) solution->state)->position->y);
    st_push ((void*) position, game.movements);
    solution = solution->parent;
  }

  return game.movements;
}

// Main search function for this module
void
tsp_search (m_stack_t places_to_visit, position2d_t *current_position)
{
  problem_t tsp_problem;
  tsp_state_t *initial_state;

  // Create a tsp initial state in memory
  initial_state = create_tsp_state(current_position->x, current_position->y);
  create_unvisited_places_stack(places_to_visit, initial_state->places_to_visit, current_position);
  printf("performing search for the tsp problem :)\n");

  // Set the problem conditions
  tsp_problem.initial_state  = initial_state;
  tsp_problem.goal_test      = tsp_visited_all_places;
  tsp_problem.actions        = tsp_movements_on_map;

  // Run A* search
  as_a_star_search (&tsp_problem, tsp_heuristic, compare_tsp_states, tsp_make_path);
}

// Get possible actions
action_t
*tsp_movements_on_map (state_t state)
{
  position2d_t *position;
  position = ((tsp_state_t*) state)->position;

  if (position->x == 0) {

    if (position->y == 0) {
      return tsp_corner_bottom_left_node;
    }

    if (position->y == (MAP_SIZE - 1)) {
      return tsp_corner_top_left_node;
    }

    return tsp_low_x_node;
  }

  if (position->x == (MAP_SIZE - 1)) {

    if (position->y == 0) {
      return tsp_corner_bottom_right_node;
    }

    if (position->y == (MAP_SIZE - 1)) {
      return tsp_corner_top_right_node;
    }

    return tsp_high_x_node;
  }

  if (position->y == (MAP_SIZE - 1)) {
    return tsp_high_y_node;
  }

  if (position->y == 0) {
    return tsp_low_y_node;
  }

  return tsp_general_node;
}

// Actions
node_t
*tsp_move_up (const state_t state)
{
  tsp_state_t *current_state, *next_state;

  current_state = (tsp_state_t*) state;
  next_state    = create_tsp_state(current_state->position->x, current_state->position->y + 1);

  // generate new stack of visited places, dealing with the new position
  create_unvisited_places_stack(current_state->places_to_visit,
                                next_state->places_to_visit,
                                next_state->position);

  return as_create_node ((state_t) next_state, movement_cost(next_state->position), NULL);
}

node_t
*tsp_move_down (const state_t state)
{
  tsp_state_t *current_state, *next_state;

  current_state = (tsp_state_t*) state;
  next_state    = create_tsp_state(current_state->position->x, current_state->position->y - 1);

  // generate new stack of visited places, dealing with the new position
  create_unvisited_places_stack(current_state->places_to_visit,
                                next_state->places_to_visit,
                                next_state->position);

  return as_create_node ((state_t) next_state, movement_cost(next_state->position), NULL);
}

node_t
*tsp_move_left (const state_t state)
{
  tsp_state_t *current_state, *next_state;

  current_state = (tsp_state_t*) state;
  next_state    = create_tsp_state(current_state->position->x - 1, current_state->position->y);

  // generate new stack of visited places, dealing with the new position
  create_unvisited_places_stack(current_state->places_to_visit,
                                next_state->places_to_visit,
                                next_state->position);

  return as_create_node ((state_t) next_state, movement_cost(next_state->position), NULL);
}

node_t
*tsp_move_right (const state_t state)
{
  tsp_state_t *current_state, *next_state;

  current_state = (tsp_state_t*) state;
  next_state    = create_tsp_state(current_state->position->x + 1, current_state->position->y);

  // generate new stack of visited places, dealing with the new position
  create_unvisited_places_stack(current_state->places_to_visit,
                                next_state->places_to_visit,
                                next_state->position);

  return as_create_node ((state_t) next_state, movement_cost(next_state->position), NULL);
}

// Goal test for this module
byte_t
tsp_visited_all_places(problem_t *problem, state_t state)
{
  return ll_is_empty(((tsp_state_t*) state)->places_to_visit);
}

// Heuristic for the tsp problem
double
tsp_heuristic(problem_t *problem, state_t current_state)
{
  return 1;
}

