#include <stdio.h>
#include <string.h>
#include <ghsgraphics.h>

// Draw grid
static void draw_grid();

// Draw unit square
static void draw_unit_square (float, float, float, float, float, float);

// Draw map
static void draw_map();

// Draw Goku
static void draw_goku();

// Draw the dragon radar view
static void draw_radar();

// Draw info on screen
static void draw_info();

// Draw fog
static void draw_fog();

// Draw screen
static void display();

// Update screen
static void update(int);

// Follow path in the movements stack
static void follow_path(int);

// Controls
static void controls(int, int, int);

// game config controls
static void configs(unsigned char, int, int);

/*
 * Controls and Drawing functions
 */

// Init openGL
void
init_opengl()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, MAP_SIZE, 0, MAP_SIZE);
  glutDisplayFunc(display);
  glutKeyboardFunc(configs);
  glutSpecialFunc(controls);
  // Timer functions
  update(0);
  follow_path(0);
}

// Config controls
static void
configs (unsigned char key, int x, int y)
{
  switch (key)
  {
    case 'g':
      // Toggle grid display
      toggle (&game.show_grid);
      break;
    case 'd':
      // Toggle "God Mode"
      toggle (&game.show_dragonballs);
      break;
    case 'i':
      // Toggle information
      toggle (&game.show_info);
      break;
    case 'r':
      // Toggle radar
      toggle (&game.show_radar);
      break;
    case ' ':
      // Restart
      restart();
      break;
    case 's':
      // Start/Stop search
      if(game.status == IDLE) {
        game.status = START;
        search();
      } else {
        // Stop search
        game.status = IDLE;
        // Clear path to be followed
        st_clear_stack(game.movements);
      }
      break;
  }
}

// Controls
static void
controls (int key, int xScreen, int yScreen)
{
  switch (key)
  {
    case GLUT_KEY_UP:
      move_goku_up();
      break;
    case GLUT_KEY_DOWN:
      move_goku_down();
      break;
    case GLUT_KEY_LEFT:
      move_goku_left();
      break;
    case GLUT_KEY_RIGHT:
      move_goku_right();
      break;
  }
}

// Draw grid
static void
draw_grid()
{
  float i;

  glColor3f (0.0f, 0.0f, 0.0f);
  glLineWidth (1.0f);
  glBegin (GL_LINES);

  for (i = 0; i < MAP_SIZE; i++) {
    glVertex2f (0, i);
    glVertex2f (MAP_SIZE, i);
    glVertex2f (i, 0);
    glVertex2f (i, MAP_SIZE);
  }

  glEnd();
}

// Helper function to draw a unit square
static void
draw_unit_square (float x, float y, float red, float green, float blue, float alpha)
{
  glColor4f (red, green, blue, alpha);
  glVertex2f (x, y);
  glVertex2f (x + 1, y);
  glVertex2f (x + 1, y + 1);
  glVertex2f (x, y + 1);
}

// Draw map
static void
draw_map()
{
  float x, y, red, green, blue;

  glBegin (GL_QUADS);

  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      switch (game.map[(byte_t)x][(byte_t)y])
      {
        case 'G':
          red    = 0.5703125f;
          green  = 0.8125f;
          blue   = 0.3125f;
          break;
        case 'M':
          red    = 0.578125f;
          green  = 0.5390625f;
          blue   = 0.328125f;
          break;
        case 'A':
          red    = 0.328125f;
          green  = 0.55078125f;
          blue   = 0.828125f;
          break;
        default:
          red = green = blue = 0.0f;
          printf ("Strange character: %d\n", game.map[(byte_t)x][(byte_t)y]);
      }

      draw_unit_square(x, y, red, green, blue, 1.0f);
    }
  }

  glEnd();
}

// Draw agent goku
static void
draw_goku()
{
  glBegin (GL_QUADS);
  draw_unit_square (game.goku.current_position.x, game.goku.current_position.y, 0.75f, 0.3125f, 0.30078125f, 1.0f);
  glEnd();
}

// Draw dragonballs
static void
draw_dragonballs()
{
  byte_t i;

  glBegin (GL_QUADS);
  
  for (i = 0; i < DRAGONBALLS_NUMBER; i++) {
    if ((game.show_dragonballs || game.dragonballs[i].seen) && (game.dragonballs[i].caught == NOT_EXPLORED)) {
      draw_unit_square (game.dragonballs[i].x, game.dragonballs[i].y, 0.98828125f, 0.60546875f, 0.02734375f, 0.8f);
    }
  }

  glEnd();
}

// Draw the dragon radar view
static void
draw_radar()
{
  // Draw white surface
  glColor4f (1.0f, 1.0f, 1.0f, 0.3f);

  glBegin (GL_QUADS);

  glVertex2f(game.goku.current_position.x + DRAGON_RADAR_DISTANCE + 1,
      game.goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(game.goku.current_position.x - DRAGON_RADAR_DISTANCE,
      game.goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(game.goku.current_position.x - DRAGON_RADAR_DISTANCE,
      game.goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(game.goku.current_position.x + DRAGON_RADAR_DISTANCE + 1,
      game.goku.current_position.y - DRAGON_RADAR_DISTANCE);

  glEnd();

  // Draw edge lines

  glColor3f(0.625f, 0.0f, 0.0f);
  glLineWidth(1.5f);

  glBegin(GL_LINES);

  // Up
  glVertex2f(game.goku.current_position.x - DRAGON_RADAR_DISTANCE,
      game.goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(game.goku.current_position.x + DRAGON_RADAR_DISTANCE + 1,
      game.goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);

  // Down
  glVertex2f(game.goku.current_position.x - DRAGON_RADAR_DISTANCE,
      game.goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(game.goku.current_position.x + DRAGON_RADAR_DISTANCE + 1,
      game.goku.current_position.y - DRAGON_RADAR_DISTANCE);

  // Left
  glVertex2f(game.goku.current_position.x - DRAGON_RADAR_DISTANCE,
      game.goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(game.goku.current_position.x - DRAGON_RADAR_DISTANCE,
      game.goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);

  // Right
  glVertex2f(game.goku.current_position.x + DRAGON_RADAR_DISTANCE + 1,
      game.goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(game.goku.current_position.x + DRAGON_RADAR_DISTANCE + 1,
      game.goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);

  glEnd();
}

// Draw info on screen
static void
draw_info()
{
  byte_t i, length;

  sprintf (info, "Custo: %d - Posicao: (%d, %d) - Dragonballs: %d", game.current_total_cost, game.goku.current_position.x, game.goku.current_position.y, game.caught_dragonballs);
  length = strlen (info);

  //glColor3f(0.0f, 0.0f, 0.0f);
  glRasterPos2f (0.0f, MAP_SIZE - 1);

  for(i = 0; i < length; i++) {
    glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, info[i]);
  }
}

// Draw fog
static void
draw_fog()
{
  byte_t x, y;
  
  glBegin (GL_QUADS);

  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      if (game.explored_map[x][y] == NOT_EXPLORED) {
        draw_unit_square(x, y, 0, 0, 0, 0.5f);
      }
    }
  }

  glEnd();
}

// Draw screen
static void
display()
{
  glClear (GL_COLOR_BUFFER_BIT);

  // Draw map
  draw_map();

  // Draw visible dragonballs
  draw_dragonballs();

  // Draw fog
  draw_fog();

  // Draw grid
  if(game.show_grid) {
    draw_grid();
  }

  // Draw radar
  if(game.show_radar) {
    draw_radar();
  }

  // Draw agent
  draw_goku();

  // Draw info
  if(game.show_info) {
    draw_info();
  }

  glutSwapBuffers();
}

// Update function
static void
update (int value)
{
  // Check if agent moved
  if ((game.goku.current_position.x != game.goku.last_position.x) ||
      (game.goku.current_position.y != game.goku.last_position.y)) {
    game.goku.last_position.x = game.goku.current_position.x;
    game.goku.last_position.y = game.goku.current_position.y;

    game.current_total_cost += movement_cost (&game.goku.current_position);
    fill_explored_map (game.explored_map, &game.goku.current_position);

    // Check to see if some dragonball was revealed
    check_seen_dragonballs();

    // Do the search!
    search();
  }

  glutPostRedisplay();
  glutTimerFunc(REFRESH_RATE, update, 0);
}

// follow path function
static void
follow_path (int value)
{
  position2d_t *new_position;

  if((game.movements != NULL) && (!ll_is_empty(game.movements))) {
    new_position = (position2d_t*) st_pop(game.movements);

    game.goku.current_position.x = new_position->x;
    game.goku.current_position.y = new_position->y;

    free (new_position);

    glutPostRedisplay();
  }

  glutTimerFunc(GOKU_SEARCH_VELOCITY, follow_path, 0);
}

