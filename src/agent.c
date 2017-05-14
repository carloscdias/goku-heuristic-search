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

// Funcao pra desenhar a tela
void display() {
  glClear(GL_COLOR_BUFFER_BIT);
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
        MAP[count_col][count_line] = (char)c;
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
    // Randomize position
    initRand();
    Goku.x = (byte)(rand() % MAP_SIZE);
    Goku.y = (byte)(rand() % MAP_SIZE);
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

