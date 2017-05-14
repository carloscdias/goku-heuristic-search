#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <smartgoku.h>

// Funcao para inicializar o rand com a hora atual do sistema
static void initRand() {
  time_t t;
  srand((unsigned) time(&t));
}

// Funcao para inicializar o openGL
void initGL() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
}

//
static void drawGrid() {
  float i;

  glColor3f(0.8f, 0.8f, 0.8f);
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

// 
static void drawUnitSquare(float x, float y, float red, float green, float blue) {
  glColor3f(red, green, blue);
  glVertex2f(x, y);
  glVertex2f(x + 1, y);
  glVertex2f(x + 1, y + 1);
  glVertex2f(x, y + 1);
}

// 
static void drawMap() {
  float x, y, red, green, blue;

  glBegin(GL_QUADS);

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      switch(MAP[(byte)x][(byte)y]) {
        case 'G':
          red = 0.0f;
          green = 1.0f;
          blue = 0.0f;
          break;
        case 'M':
          red = 1.0f;
          green = 0.5f;
          blue = 0.5f;
          break;
        case 'A':
          red = 0.0f;
          green = 0.0f;
          blue = 1.0f;
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

//
static void drawGoku() {
  glBegin(GL_QUADS);
  drawUnitSquare(Goku.x, Goku.y, 1.0f, 0.0f, 0.0f);
  glEnd();
}

// Funcao pra desenhar a tela
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  drawMap();
  drawGoku();
  drawGrid();
  glutSwapBuffers();
}

// Funcao para redesenhar a tela
void reshape(int w, int h) {
  glViewport(0, 0, w, h);
}

// Funcao para ler o mapa de um arquivo
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

// Funcao para inicializar o agente
void initAgent(Position2D *position) {
  if (position != NULL) {
    Goku.x = position->x;
    Goku.y = position->y;
  } else {
    Goku.x = 19;
    Goku.y = 22;
  }
}

// Funcao para inicializar as esferas do dragao
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

/*Funcao para mover o agente para esquerda uma vez*/
void moveLeft() {
  if (Goku.x > 0) {
    Goku.x--;
  }
}

/*Funcao para mover o agente para direita uma vez*/
void moveRight() {
  if (Goku.x < MAP_SIZE) {
    Goku.x++;
  }
}

/*Funcao para mover o agente para cima uma vez*/
void moveUp() {
  if (Goku.y > 0) {
    Goku.y--;
  }
}

/*Funcao para mover o agente para baixo uma vez*/
void moveDown() {
  if (Goku.y < MAP_SIZE) {
    Goku.y++;
  }
}

