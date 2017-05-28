#include <stdlib.h>
#include <stdio.h>
#include <ghsdata.h>
#include <stack.h>
#include <pathsearch.h>
#include <pathexploresearch.h>

stack_t explore_path = NULL;
position2d_t *saved_pursuit = NULL;

// Load path file to memory
void
pes_init_explore (char *path_file)
{
  position2d_t position, *tmp;
  FILE *file;

  explore_path = st_create_stack();
  file = fopen (path_file, "rb");
  
  if (file) {
    while (fread(&position, sizeof(position2d_t), 1, file)) {
      tmp = create_position(position.x, position.y);
      st_push((void*)tmp, explore_path);
    }

    fclose(file);
  }
}

// Explore map search
void
pes_explore_search()
{
  position2d_t *position = NULL;

  // Check if has any saved instance
  if (saved_pursuit != NULL) {
    position = saved_pursuit;
    saved_pursuit = NULL;
  } else if (!ll_is_empty(explore_path)) {
    position = st_pop(explore_path);
  }

  // Do the movement
  if (position != NULL) {
    path_search (game.goku.current_position.x, game.goku.current_position.y, position->x, position->y);
    free(position);
  }
}

// Save current pursuit
void pes_save_explore() {
  if (ll_is_empty(movements)) {
    return;
  }
  
  while (movements->number_of_elements > 1) {
    saved_pursuit = (position2d_t*) st_pop(game.movements);
    free(saved_pursuit);
  }

  saved_pursuit = (position2d_t*) st_pop(game.movements);
}

