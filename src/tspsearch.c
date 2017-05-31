#include <tspsearch.h>

// Actions
// action_t general_node[5]              = {ps_move_up,   ps_move_down,  ps_move_left,  ps_move_right, NULL};
// action_t low_x_node[4]                = {ps_move_up,   ps_move_down,  ps_move_right, NULL};
// action_t low_y_node[4]                = {ps_move_up,   ps_move_left,  ps_move_right, NULL};
// action_t high_x_node[4]               = {ps_move_up,   ps_move_down,  ps_move_left,  NULL};
// action_t high_y_node[4]               = {ps_move_down, ps_move_left,  ps_move_right, NULL};
// action_t corner_top_left_node[3]      = {ps_move_down, ps_move_right, NULL};
// action_t corner_top_right_node[3]     = {ps_move_down, ps_move_left,  NULL};
// action_t corner_bottom_left_node[3]   = {ps_move_up,   ps_move_right, NULL};
// action_t corner_bottom_right_node[3]  = {ps_move_up,   ps_move_left,  NULL};

// Push solution to the movements stack
void
*tsp_make_path (node_t *solution)
{
  position2d_t *position;

  if (!ll_is_empty (game.movements)) {
    st_clear_stack (game.movements);
  }

  while (solution != NULL) {
    position = create_position (((tsp_state_t*) solution->state)->position.x, ((tsp_state_t*) solution->state)->position.y);
    st_push ((void*) position, game.movements);
    solution = solution->parent;
  }

  return game.movements;
}

// Main search function for this module
void
*tsp_search (m_stack_t places_to_visit, position2d_t *current_position)
{
  problem_t tsp_problem;

  path_problem.initial_state  = create_position (x1, y1);
  path_problem.final_state    = create_position (x2, y2);
  path_problem.goal_test      = ps_reached_destination;
  path_problem.actions        = ps_movements_on_map;

  as_a_star_search (&tsp_problem, tsp_heuristic, "COMPARE STATES", tsp_make_path);

  free (path_problem.final_state);
}

// Get possible actions
action_t
*ps_movements_on_map (state_t state)
{
  position2d_t *position;
  position = (position2d_t*) state;

  if (position->x == 0) {

    if (position->y == 0) {
      return corner_bottom_left_node;
    }

    if (position->y == (MAP_SIZE - 1)) {
      return corner_top_left_node;
    }

    return low_x_node;
  }

  if (position->x == (MAP_SIZE - 1)) {

    if (position->y == 0) {
      return corner_bottom_right_node;
    }

    if (position->y == (MAP_SIZE - 1)) {
      return corner_top_right_node;
    }

    return high_x_node;
  }

  if (position->y == (MAP_SIZE - 1)) {
    return high_y_node;
  }

  if (position->y == 0) {
    return low_y_node;
  }

  return general_node;
}

// Actions
node_t
*ps_move_up (const state_t state)
{
  position2d_t *position, *new_position;
  position = (position2d_t*) state;

  new_position = create_position (position->x, position->y + 1);

  return as_create_node ((state_t) new_position, movement_cost (new_position), NULL);
}

node_t
*ps_move_down (const state_t state)
{
  position2d_t *position, *new_position;
  position = (position2d_t*) state;

  new_position = create_position (position->x, position->y - 1);

  return as_create_node((state_t) new_position, movement_cost (new_position), NULL);
}

node_t
*ps_move_left (const state_t state)
{
  position2d_t *position, *new_position;
  position = (position2d_t*) state;

  new_position = create_position (position->x - 1, position->y);

  return as_create_node((state_t) new_position, movement_cost (new_position), NULL);
}

node_t
*ps_move_right (const state_t state)
{
  position2d_t *position, *new_position;
  position = (position2d_t*) state;

  new_position = create_position (position->x + 1, position->y);

  return as_create_node((state_t) new_position, movement_cost (new_position), NULL);
}

// Goal test for this module
byte_t
visited_all_places(problem_t *problem, state_t state)
{
  return ll_is_empty(((tsp_state_t*) state)->places_to_visit);
}

