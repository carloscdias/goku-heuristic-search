#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ghsdata.h>
#include <pathsearch.h>
#include <rateexploresearch.h>
#include <smartgoku.h>

// Init rand seed
static void init_rand();

// Init agent position
static void init_agent(position2d_t*);

// Init dragonballs position
static void init_dragonballs(char*);

// Init rand with current time
static void
init_rand()
{
  time_t t;
  srand((unsigned) time(&t));
}

// Init game global variable
void
init_game(char *mapfile, char *pathfile)
{
  // init visual configs
  game.show_info           = 1;
  game.show_dragonballs    = 0;
  game.show_grid           = 1;
  game.show_radar          = 1;
  // Init stacks
  game.movements           = st_create_stack();
  game.seen_dragonballs    = st_create_stack();
  game.tracked_dragonballs = st_create_stack();
  // Init map
  init_map(mapfile);
  // Init other variables
  restart();
}

// Restart game global variable
void
restart()
{
  // Clear stacks
  st_clear_stack(game.movements);
  st_clear_stack(game.seen_dragonballs);
  st_clear_stack(game.tracked_dragonballs);
  
  // Reset variables
  game.caught_dragonballs = game.current_total_cost = 0;
  game.status = IDLE;

  // Reinit dragonballs
  init_dragonballs (NULL);
  
  // Reinit agent goku
  init_agent (NULL);

  // Reinit explored map
  init_explored_map (game.explored_map);
  fill_explored_map (game.explored_map, &game.goku.current_position);

  // Check if any dragonball is visible
  check_seen_dragonballs();
}

// Read map from file
void
init_map (char *filename)
{
  int c;
  int count_line = MAP_SIZE - 1;
  int count_col = 0;
  FILE *file;

  file = fopen(filename, "r");
  
  if (file) {
    while ((c = getc(file)) != EOF) {
      if (count_col == MAP_SIZE) {
        count_line--;
        count_col = 0;
      } else {
        game.map[count_col][count_line] = (byte_t)c;
        count_col++;
      }
    }

    fclose(file);
  }
}

// Init agent position
static void
init_agent (position2d_t *position)
{
  if (position != NULL) {
    game.goku.last_position.x = game.goku.current_position.x = position->x;
    game.goku.last_position.y = game.goku.current_position.y = position->y;
  } else {
    game.goku.last_position.x = game.goku.current_position.x = 19;
    game.goku.last_position.y = game.goku.current_position.y = 22;
  }
}

// Init dragonballs position
static void
init_dragonballs(char *positions)
{
  byte_t i;

  init_rand();

  for (i = 0; i < DRAGONBALLS_NUMBER; i++) {
    if (positions != NULL && positions[i] != 0) {
      // dragonballs[i].x = positions[i]->x;
      // dragonballs[i].y = positions[i]->y;
    } else {
      game.dragonballs[i].x = (byte_t)(rand() % MAP_SIZE);
      game.dragonballs[i].y = (byte_t)(rand() % MAP_SIZE);
    }

    game.dragonballs[i].seen = game.dragonballs[i].caught = 0;
  }
}

// Remove dragonball from stack of tracked ones
static void
remove_tracked_dragonball (dragonball_t *dragonball)
{
  place_t *current;
  linkedlistnode_t *tmp, *previous;

  // Check if dragonball is the first one
  current = (place_t*) game.tracked_dragonballs->first_node->data;
  if ((current->position.x == dragonball->x) && (current->position.y == dragonball->y)) {
    // it is, remove this one
    tmp = ll_remove_first_node(game.tracked_dragonballs);
    free (tmp->data);
    free (tmp);
    return;
  }

  // Check if dragonball is the last one
  current = (place_t*) game.tracked_dragonballs->last_node->data;
  if ((current->position.x == dragonball->x) && (current->position.y == dragonball->y)) {
    tmp = ll_remove_last_node(game.tracked_dragonballs);
    free (tmp->data);
    free (tmp);
    return;
  }

  // It's in the middle
  previous = game.tracked_dragonballs->first_node;
  tmp = previous->next;
  while (tmp != NULL) {
    current = (place_t*) tmp->data;

    if ((current->position.x == dragonball->x) && (current->position.y == dragonball->y)) {
      previous->next = tmp->next;
      game.tracked_dragonballs->number_of_elements--;
      free (current);
      free (tmp);
      return;
    }

    previous = tmp;
    tmp = tmp->next;
  }
}

// Check if any dragonball is inside dragonradar
void
check_seen_dragonballs()
{
  place_t *dragonball_to_catch;
  byte_t i;

  for (i = 0; i < DRAGONBALLS_NUMBER; i++) {
    if (game.explored_map[game.dragonballs[i].x][game.dragonballs[i].y] == EXPLORED) {
      if (game.dragonballs[i].seen == NOT_EXPLORED) {
        game.dragonballs[i].seen = EXPLORED;

        // Add to stack
        dragonball_to_catch = create_place(game.dragonballs[i].x, game.dragonballs[i].y, NOT_EXPLORED);
        st_push((void*) dragonball_to_catch, game.seen_dragonballs);
      }

      if ((game.dragonballs[i].caught == NOT_EXPLORED) &&
          (game.dragonballs[i].x == game.goku.current_position.x) &&
          (game.dragonballs[i].y == game.goku.current_position.y)) {
        // If is searching
        if (game.status == START) {
          // Remove catched dragonball from stack of tracked ones
          remove_tracked_dragonball(&game.dragonballs[i]);
        }
        game.dragonballs[i].caught = EXPLORED;
        game.caught_dragonballs++;
      }
    }
  }
}

// Move agent left one time
void
move_goku_left()
{
  if (game.goku.current_position.x > 0) {
    game.goku.current_position.x--;
  }
}

// Move agent right one time
void
move_goku_right()
{
  if (game.goku.current_position.x < MAP_SIZE - 1) {
    game.goku.current_position.x++;
  }
}

// Move agent up one time
void
move_goku_up()
{
  if (game.goku.current_position.y < MAP_SIZE - 1) {
    game.goku.current_position.y++;
  }
}

// Move agent down one time
void
move_goku_down()
{
  if (game.goku.current_position.y > 0) {
    game.goku.current_position.y--;
  }
}

// Search algorithm
void
search()
{
  place_t *dragonball_to_catch;

  // IDLE means that search is OFF
  if (game.status == IDLE) {
    return;
  }

  // If already caught all dragonballs
  if (game.caught_dragonballs == DRAGONBALLS_NUMBER) {
    // Finish search and goes back to origin
    game.status = IDLE;
    ps_path_search(game.goku.current_position.x, game.goku.current_position.y, 19, 22);
    return;
  }

  // If there isn't any visible dragonball
  if(ll_is_empty(game.movements) && game.seen_dragonballs->number_of_elements == 0) {
    // Performs search
    res_explore_search();
  } else if(game.seen_dragonballs->number_of_elements > 0) {
    // Check if any dragonball is already being tracked
    if (game.tracked_dragonballs->number_of_elements == 0) {
      // There isn't dragonballs being tracked
      // Check if only one dragonball was seen now
      if (game.seen_dragonballs->number_of_elements == 1) {
        // Add this dragonball to stack of tracked
        // ones and perform search to get it
        dragonball_to_catch = st_pop(game.seen_dragonballs);
        st_push(dragonball_to_catch, game.tracked_dragonballs);
        // Perform search
        ps_path_search(game.goku.current_position.x, game.goku.current_position.y,
            dragonball_to_catch->position.x, dragonball_to_catch->position.y);
      } else {
        // More than one dragonball was seen
        // Add all of them to stack of tracked ones
        while (!ll_is_empty(game.seen_dragonballs)) {
          dragonball_to_catch = st_pop(game.seen_dragonballs);
          st_push(dragonball_to_catch, game.tracked_dragonballs);
        }
        // Perform multiple search algorithm
        printf("Achou mais de uma ao mesmo tempo!\n");
      }
    } else {
      // There is some dragonball being tracked
      // Add all of the seen dragonballs to stack of tracked ones
      while (!ll_is_empty(game.seen_dragonballs)) {
        dragonball_to_catch = st_pop(game.seen_dragonballs);
        st_push(dragonball_to_catch, game.tracked_dragonballs);
      }
      // Perform multisearch
      printf("Já havia uma busca em andamento, mas outra esfera foi localizada! :o\n");
    }
  }
}

