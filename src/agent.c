#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#ifdef __gnu_linux__
#include <GL/gl.h>
#include <GL/freeglut.h>
#endif
#include <utils.h>
#include <pathsearch.h>
#include <exploresearch.h>
#include <smartgoku.h>

static void update(int);
static void followPath(int);
static void display();

// Init rand with current time
static void initRand() {
  time_t t;
  srand((unsigned) time(&t));
}

// Helper to toggle a value
static void toggle(byte *value) {
  if (*value == 0) {
    *value = 1;
  } else {
    *value = 0;
  }
}

// Init board
void initBoard() {
  board.showInfo = 1;
  board.showDragonballs = 0;
  board.showGrid = 1;
  board.showDragonRadar = 1;
  board.currentTotalCost = 0;
  board.caughtDragonballs = 0;
}

// Controls
static void controls(int key, int xScreen, int yScreen) {
  switch(key) {
    case GLUT_KEY_UP:
      moveGokuUp();
      break;
    case GLUT_KEY_DOWN:
      moveGokuDown();
      break;
    case GLUT_KEY_LEFT:
      moveGokuLeft();
      break;
    case GLUT_KEY_RIGHT:
      moveGokuRight();
      break;
  }
}

// Config controls
static void configs(unsigned char key, int x, int y) {
  switch(key) {
    case 'g':
      // Toggle grid display
      toggle(&board.showGrid);
      break;
    case 'd':
      // Toggle "God Mode"
      toggle(&board.showDragonballs);
      break;
    case 'i':
      // Toggle information
      toggle(&board.showInfo);
      break;
    case 'r':
      // Toggle radar
      toggle(&board.showDragonRadar);
      break;
    case ' ':
      // Restart
      board.caughtDragonballs = board.currentTotalCost = 0;
      initDragonballs(NULL);
      initAgent(NULL);
      init_explored_map(EXPLORED_MAP);
      fill_explored_map(EXPLORED_MAP, &goku.current_position);
      checkSeenDragonballs();
      break;
    case 's':
      // Start/Stop search following the path
      // explore_search();
      printf("Cost: %d\n", path_cost(goku.current_position.x, goku.current_position.y, 40, 7));
      path_search(goku.current_position.x, goku.current_position.y, 40, 7);
      break;
  }
}

// Read map from file
void initMap(char *filename)
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
        MAP[count_col][count_line] = (byte)c;
        count_col++;
      }
    }

    fclose(file);
  }
}

// Init agent position
void initAgent(Position2D *position) {
  if (position != NULL) {
    goku.last_position.x = goku.current_position.x = position->x;
    goku.last_position.y = goku.current_position.y = position->y;
  } else {
    goku.last_position.x = goku.current_position.x = 19;
    goku.last_position.y = goku.current_position.y = 22;
  }
}

// Init dragonballs position
void initDragonballs(Position2D *positions[]) {
  byte i;

  initRand();

  for(i = 0; i < DRAGONBALLS_NUMBER; i++) {
    if(positions != NULL && positions[i] != NULL) {
      dragonballs[i].x = positions[i]->x;
      dragonballs[i].y = positions[i]->y;
    } else {
      dragonballs[i].x = (byte)(rand() % MAP_SIZE);
      dragonballs[i].y = (byte)(rand() % MAP_SIZE);
    }

    dragonballs[i].seen = dragonballs[i].caught = 0;
  }
}

// Check if any dragonball is inside dragonradar
void checkSeenDragonballs() {
  byte i;

  for(i = 0; i < DRAGONBALLS_NUMBER; i++) {
    if(EXPLORED_MAP[dragonballs[i].x][dragonballs[i].y]) {
      dragonballs[i].seen = EXPLORED;

      if((dragonballs[i].caught == NOT_EXPLORED) && (dragonballs[i].x == goku.current_position.x) && (dragonballs[i].y == goku.current_position.y)) {
        dragonballs[i].caught = EXPLORED;
        board.caughtDragonballs++;
      }
    }
  }
}

// Move agent left one time
void moveGokuLeft() {
  if (goku.current_position.x > 0) {
    goku.current_position.x--;
  }
}

// Move agent right one time
void moveGokuRight() {
  if (goku.current_position.x < MAP_SIZE - 1) {
    goku.current_position.x++;
  }
}

// Move agent up one time
void moveGokuUp() {
  if (goku.current_position.y < MAP_SIZE - 1) {
    goku.current_position.y++;
  }
}

// Move agent down one time
void moveGokuDown() {
  if (goku.current_position.y > 0) {
    goku.current_position.y--;
  }
}

/*
 * Drawing functions
 */
// Init openGL
void initGL() {
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
  followPath(0);
}

// Draw grid
static void drawGrid() {
  float i;

  glColor3f(0.0f, 0.0f, 0.0f);
  glLineWidth(1.0f);
  glBegin(GL_LINES);

  for(i = 0; i < MAP_SIZE; i++) {
    glVertex2f(0, i);
    glVertex2f(MAP_SIZE, i);
    glVertex2f(i, 0);
    glVertex2f(i, MAP_SIZE);
  }

  glEnd();
}

// Helper function to draw a unit square
static void drawUnitSquare(float x, float y, float red, float green, float blue, float alpha) {
  glColor4f(red, green, blue, alpha);
  glVertex2f(x, y);
  glVertex2f(x + 1, y);
  glVertex2f(x + 1, y + 1);
  glVertex2f(x, y + 1);
}

// Draw map
static void drawMap() {
  float x, y, red, green, blue;

  glBegin(GL_QUADS);

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      switch(MAP[(byte)x][(byte)y]) {
        case 'G':
          red = 0.5703125f;
          green = 0.8125f;
          blue = 0.3125f;
          break;
        case 'M':
          red = 0.578125f;
          green = 0.5390625f;
          blue = 0.328125f;
          break;
        case 'A':
          red = 0.328125f;
          green = 0.55078125f;
          blue = 0.828125f;
          break;
        default:
          red = green = blue = 0.0f;
          printf("Strange character: %d\n", MAP[(byte)x][(byte)y]);
      }

      drawUnitSquare(x, y, red, green, blue, 1.0f);
    }
  }

  glEnd();
}

// Draw agent goku
static void drawGoku() {
  glBegin(GL_QUADS);
  drawUnitSquare(goku.current_position.x, goku.current_position.y, 0.75f, 0.3125f, 0.30078125f, 1.0f);
  glEnd();
}

// Draw dragonballs
static void drawDragonballs() {
  byte i;

  glBegin(GL_QUADS);
  
  for(i = 0; i < DRAGONBALLS_NUMBER; i++) {
    if((board.showDragonballs || dragonballs[i].seen) && (dragonballs[i].caught == NOT_EXPLORED)) {
      drawUnitSquare(dragonballs[i].x, dragonballs[i].y, 0.98828125f, 0.60546875f, 0.02734375f, 0.8f);
    }
  }

  glEnd();
}

// Draw the dragon radar view
static void drawDragonRadar() {
  // Draw white surface
  glColor4f(1.0f, 1.0f, 1.0f, 0.3f);

  glBegin(GL_QUADS);

  glVertex2f(goku.current_position.x + DRAGON_RADAR_DISTANCE + 1, goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(goku.current_position.x - DRAGON_RADAR_DISTANCE, goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(goku.current_position.x - DRAGON_RADAR_DISTANCE, goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(goku.current_position.x + DRAGON_RADAR_DISTANCE + 1, goku.current_position.y - DRAGON_RADAR_DISTANCE);

  glEnd();

  // Draw edge lines

  glColor3f(0.625f, 0.0f, 0.0f);
  glLineWidth(1.5f);

  glBegin(GL_LINES);

  // Up
  glVertex2f(goku.current_position.x - DRAGON_RADAR_DISTANCE, goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(goku.current_position.x + DRAGON_RADAR_DISTANCE + 1, goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);

  // Down
  glVertex2f(goku.current_position.x - DRAGON_RADAR_DISTANCE, goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(goku.current_position.x + DRAGON_RADAR_DISTANCE + 1, goku.current_position.y - DRAGON_RADAR_DISTANCE);

  // Left
  glVertex2f(goku.current_position.x - DRAGON_RADAR_DISTANCE, goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(goku.current_position.x - DRAGON_RADAR_DISTANCE, goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);

  // Right
  glVertex2f(goku.current_position.x + DRAGON_RADAR_DISTANCE + 1, goku.current_position.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(goku.current_position.x + DRAGON_RADAR_DISTANCE + 1, goku.current_position.y + DRAGON_RADAR_DISTANCE + 1);

  glEnd();
}

// Draw info on screen
static void drawText() {
  byte i, length;

  sprintf(text, "Custo: %d - Posicao: (%d, %d) - Dragonballs: %d", board.currentTotalCost, goku.current_position.x, goku.current_position.y, board.caughtDragonballs);
  length = strlen(text);

  //glColor3f(0.0f, 0.0f, 0.0f);
  glRasterPos2f(0.0f, MAP_SIZE - 1);

  for(i = 0; i < length; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
  }
}

// Draw fog
static void drawFog() {
  byte x, y;
  
  glBegin(GL_QUADS);

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      if(EXPLORED_MAP[x][y] == NOT_EXPLORED) {
        drawUnitSquare(x, y, 0, 0, 0, 0.5f);
      }
    }
  }

  glEnd();
}

// Draw screen
static void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw map
  drawMap();

  // Draw visible dragonballs
  drawDragonballs();

  // Draw fog
  drawFog();

  // Draw grid
  if(board.showGrid) {
    drawGrid();
  }

  // Draw radar
  if(board.showDragonRadar) {
    drawDragonRadar();
  }

  // Draw agent
  drawGoku();

  // Draw info
  if(board.showInfo) {
    drawText();
  }

  glutSwapBuffers();
}

// Update function
static void update(int value) {
  // Check if agent moved
  if((goku.current_position.x != goku.last_position.x) || (goku.current_position.y != goku.last_position.y)) {
    goku.last_position.x = goku.current_position.x;
    goku.last_position.y = goku.current_position.y;

    board.currentTotalCost += movement_cost(&goku.current_position);
    fill_explored_map(EXPLORED_MAP, &goku.current_position);

    // Check to see if some dragonball was revealed
    checkSeenDragonballs();
  }

  glutPostRedisplay();
  glutTimerFunc(REFRESH_RATE, update, 0);
}

// follow path function
static void followPath(int value) {
  Position2D *new_position;

  if((movements != NULL) && (!ll_is_empty(movements))) {
    new_position = (Position2D*) st_pop(movements);

    goku.current_position.x = new_position->x;
    goku.current_position.y = new_position->y;

    free(new_position);

    glutPostRedisplay();
  }

  glutTimerFunc(GOKU_SEARCH_VELOCITY, followPath, 0);
}

