#include <stdlib.h>
#include <tspsearch.h>

// Static functions declarations

// Helper to create the state
static tsp_state_t *create_tsp_state(byte_t, byte_t);

// Create a new stack of unvisited places based on the old one and the new position
static void create_unvisited_places_stack(m_stack_t, m_stack_t, position2d_t*);

// Check if position is in stack
static byte_t tsp_is_in_stack(position2d_t*, m_stack_t);

// Compare 2 tsp states for equality
static byte_t compare_tsp_states(void*, void *);

// Simple manhatan distance
static unsigned int tsp_manhatan_distance(position2d_t*, position2d_t*);

// Heuristic recursion
static unsigned int tsp_calculate_heuristic(position2d_t*, m_stack_t);


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

// Check if given position is in the given stack
static byte_t
tsp_is_in_stack(position2d_t *position, m_stack_t stack)
{
  linkedlistnode_t *node;

  if (ll_is_empty(stack)) {
    return NOT_EXPLORED;
  }

  // Check stack looking for position
  node = stack->first_node;
  while (node != NULL) {
    if (compare_positions(position, (position2d_t*) node->data)) {
      return EXPLORED;
    }

    node = node->next;
  }

  return NOT_EXPLORED;
}

// Function to compare two tsp_states and returns
// 1 if they are equals or 0 if they are different
static byte_t
compare_tsp_states(void *tsp1, void *tsp2)
{
  tsp_state_t *state1, *state2;
  linkedlistnode_t *node;
  
  state1 = (tsp_state_t*) tsp1;
  state2 = (tsp_state_t*) tsp2;

  // If positions are diferent, they are not equals
  if (compare_positions(state1->position, state2->position) == NOT_EXPLORED) {
    return NOT_EXPLORED;
  }

  // If both are empty, they are equals
  if (ll_is_empty(state1->places_to_visit) && ll_is_empty(state2->places_to_visit)) {
    return EXPLORED;
  }

  // If they have different number of elements in the stack, they are not equals
  if (state1->places_to_visit->number_of_elements != state2->places_to_visit->number_of_elements) {
    return NOT_EXPLORED;
  }

  node = state1->places_to_visit->first_node;
  while (node != NULL) {
    // If there is at least one element different, they are differents
    if (tsp_is_in_stack((position2d_t*) node->data, state2->places_to_visit) == NOT_EXPLORED) {
      return NOT_EXPLORED;
    }

    node = node->next;
  }

  // Otherwise, they are equals
  return EXPLORED;
}

// TSP manhatan distance
static unsigned int
tsp_manhatan_distance(position2d_t *position1, position2d_t *position2)
{
  return (abs (position1->x - position2->x) + abs (position1->y - position2->y));
}

// Function to calculate the heuristic recursively
static unsigned int
tsp_calculate_heuristic(position2d_t *position, m_stack_t stack)
{
  linkedlistnode_t *current_best, *tmp;
  unsigned int min_distance, tmp_distance, i, number_of_nodes;

  if (stack->number_of_elements == 0) {
    return 0;
  }

  // Pop the first element
  current_best = ll_remove_first_node(stack);
  min_distance = tsp_manhatan_distance(position, (position2d_t*) current_best->data);

  number_of_nodes = stack->number_of_elements;

  // loop through all nodes
  for (i = 0; i < number_of_nodes; i++) {
    tmp = ll_remove_first_node(stack);
    tmp_distance = tsp_manhatan_distance(position, (position2d_t*) tmp->data);
    
    // Change min value and reinsert earlier min node to the end of the linked list
    if (tmp_distance < min_distance) {
      min_distance = tmp_distance;
      ll_insert_last_node(current_best, stack);
      current_best = tmp;
    } else {
      // reinsert node to the end of the list
      ll_insert_last_node(tmp, stack);
    }
  }

  return min_distance + tsp_calculate_heuristic((position2d_t*) current_best->data, stack);
}

// Helper function to create a node based on x, y and current state
static node_t
*tsp_create_node(int x, int y, state_t state)
{
  tsp_state_t *current_state, *next_state;

  current_state = (tsp_state_t*) state;
  next_state    = create_tsp_state(current_state->position->x + x, current_state->position->y + y);

  // generate new stack of visited places, dealing with the new position
  create_unvisited_places_stack(current_state->places_to_visit,
                                next_state->places_to_visit,
                                next_state->position);

  return as_create_node ((state_t) next_state, movement_cost(next_state->position), NULL);
}

// Push solution to the movements stack
void
*tsp_make_path (node_t *solution)
{
  position2d_t *position;

  if (!ll_is_empty (game.movements)) {
    st_clear_stack (game.movements);
  }

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
// Action move up
node_t
*tsp_move_up (const state_t state)
{
  return tsp_create_node(0, 1, state);
}

// Action move down
node_t
*tsp_move_down (const state_t state)
{
  return tsp_create_node(0, -1, state);
}

// Action move left
node_t
*tsp_move_left (const state_t state)
{
  return tsp_create_node(-1, 0, state);
}

// Action move right
node_t
*tsp_move_right (const state_t state)
{
  return tsp_create_node(1, 0, state);
}

// Goal test for this module
byte_t
tsp_visited_all_places(problem_t *problem, state_t state)
{
  return ll_is_empty(((tsp_state_t*) state)->places_to_visit);
}

// Heuristic for the tsp problem based on the minimum spanning trees
double
tsp_heuristic(problem_t *problem, state_t current_state)
{
  tsp_state_t *state;
  m_stack_t stack_copy;
  linkedlistnode_t *node;
  position2d_t *position;
  unsigned int heuristic;

  state = (tsp_state_t*) current_state;

  // Copy stack
  stack_copy = st_create_stack();
  node = state->places_to_visit->first_node;
  while (node != NULL) {
    position = create_position(((position2d_t*) node->data)->x, ((position2d_t*) node->data)->y);
    st_push(position, stack_copy);
    node = node->next;
  }

  // Calculate heuristic
  heuristic = tsp_calculate_heuristic(state->position, stack_copy);
  st_destroy_stack(stack_copy);

  return heuristic;
}

