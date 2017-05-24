#include <stdlib.h>
#include <stdio.h>
#include <smartgoku.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#endif
#ifdef __gnu_linux__
#include <GL/gl.h>
#include <GL/freeglut.h>
#endif

#define OK  0 

// Init global vars
Agent goku;
Dragonball dragonballs[DRAGONBALLS_NUMBER];

byte MAP[MAP_SIZE][MAP_SIZE];
byte EXPLORED_MAP[MAP_SIZE][MAP_SIZE];

Stack movements = NULL;

char text[TEXT_BUFFER];

BoardSetup board;

// Main function
int main(int argc, char *argv[], char *envp[]) {	
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(630, 630);
  glutInitWindowPosition(350, 50);
  glutCreateWindow("Goku Heuristic Search");

  initBoard();
  initMap("Mapa01.txt");
  initAgent(NULL);
  init_explored_map(EXPLORED_MAP);
  fill_explored_map(EXPLORED_MAP, &goku.current_position);
  initDragonballs(NULL);
  initGL();

  glutMainLoop();

	return OK;
}

