#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#ifdef __APPLE__ && __MACH__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#ifdef __gnu_linux__
#include <GL/gl.h>
#include <GL/freeglut.h>
#endif
#include <smartgoku.h>

// Init rand with current time
static void initRand() {
  time_t t;
  srand((unsigned) time(&t));
}

// Init openGL
void initGL() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, MAP_SIZE, 0, MAP_SIZE);
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
static void drawUnitSquare(float x, float y, float red, float green, float blue) {
  glColor3f(red, green, blue);
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

      drawUnitSquare(x, MAP_SIZE - y - 1, red, green, blue);
    }
  }

  glEnd();
}

// Draw agent goku
static void drawGoku() {
  glBegin(GL_QUADS);
  drawUnitSquare(Goku.x, Goku.y, 0.75f, 0.3125f, 0.30078125f);
  glEnd();
}

// Draw dragonballs
static void drawDragonballs() {
  byte i;

  glBegin(GL_QUADS);
  
  for(i = 0; i < DRAGONBALLS_NUMBER; i++) {
    drawUnitSquare(Dragonballs[i].x, Dragonballs[i].y, 0.98828125f, 0.60546875f, 0.02734375f);
  }

  glEnd();
}

// Draw the dragon radar view
static void drawDragonRadar() {
  // Draw white surface
  /*
  glColor4f(1.0f, 1.0f, 1.0f, 0.3f);

  glBegin(GL_QUADS);

  glVertex2f(Goku.x + DRAGON_RADAR_DISTANCE + 1, Goku.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(Goku.x - DRAGON_RADAR_DISTANCE, Goku.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(Goku.x - DRAGON_RADAR_DISTANCE, Goku.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(Goku.x + DRAGON_RADAR_DISTANCE + 1, Goku.y - DRAGON_RADAR_DISTANCE);

  glEnd();
  */

  // Draw edge lines

  glColor3f(0.625f, 0.0f, 0.0f);
  glLineWidth(1.5f);

  glBegin(GL_LINES);

  // Up
  glVertex2f(Goku.x - DRAGON_RADAR_DISTANCE, Goku.y + DRAGON_RADAR_DISTANCE + 1);
  glVertex2f(Goku.x + DRAGON_RADAR_DISTANCE + 1, Goku.y + DRAGON_RADAR_DISTANCE + 1);

  // Down
  glVertex2f(Goku.x - DRAGON_RADAR_DISTANCE, Goku.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(Goku.x + DRAGON_RADAR_DISTANCE + 1, Goku.y - DRAGON_RADAR_DISTANCE);

  // Left
  glVertex2f(Goku.x - DRAGON_RADAR_DISTANCE, Goku.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(Goku.x - DRAGON_RADAR_DISTANCE, Goku.y + DRAGON_RADAR_DISTANCE + 1);

  // Right
  glVertex2f(Goku.x + DRAGON_RADAR_DISTANCE + 1, Goku.y - DRAGON_RADAR_DISTANCE);
  glVertex2f(Goku.x + DRAGON_RADAR_DISTANCE + 1, Goku.y + DRAGON_RADAR_DISTANCE + 1);

  glEnd();
}

// Draw points from which the whole map can be seen by the dragon radar
static void drawInterestingPoints() {
}

//
static void drawText() {
  byte i;

  //glColor3f(0.0f, 0.0f, 0.0f);
  glRasterPos2f(0.0f, MAP_SIZE - 1);

  for(i = 0; i < TEXT_BUFFER; i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
  }
}

// Draw screen
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawMap();
  drawDragonballs();
  drawGoku();
  drawGrid();
  drawDragonRadar();
  drawText();
  glutSwapBuffers();
}

// Read map from file
void initMap(char *filename)
{
  int c;
  int count_line = 0;
  int count_col = 0;
  FILE *file;

  file = fopen(filename, "r");
  
  if (file) {
    while ((c = getc(file)) != EOF) {
      if (count_col == MAP_SIZE) {
        count_line++;
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
    Goku.x = position->x;
    Goku.y = position->y;
  } else {
    Goku.x = 19;
    Goku.y = 22;
  }
}

// Init dragonballs position
void initDragonballs(Position2D *positions[]) {
  byte i;

  initRand();

  for(i = 0; i < DRAGONBALLS_NUMBER; i++) {
    if(positions != NULL && positions[i] != NULL) {
      Dragonballs[i].x = positions[i]->x;
      Dragonballs[i].y = positions[i]->y;
    } else {
      Dragonballs[i].x = (byte)(rand() % MAP_SIZE);
      Dragonballs[i].y = (byte)(rand() % MAP_SIZE);
    }
  }
}

// Move agent left one time
void moveLeft() {
  if (Goku.x > 0) {
    Goku.x--;
  }
}

// Move agent right one time
void moveRight() {
  if (Goku.x < MAP_SIZE) {
    Goku.x++;
  }
}

// Move agent up one time
void moveUp() {
  if (Goku.y > 0) {
    Goku.y--;
  }
}

// Move agent down one time
void moveDown() {
  if (Goku.y < MAP_SIZE) {
    Goku.y++;
  }
}

