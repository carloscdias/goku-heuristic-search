#include <stdlib.h>
#include <stdio.h>
#include <smartgoku.h>
#include <gokusearch.h>
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
Position2D Goku;
Position2D Dragonballs[DRAGONBALLS_NUMBER];

char text[TEXT_BUFFER];

BoardSetup board;

// Main function
int main(int argc, char *argv[], char *envp[]) {	
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(630, 630);
  glutInitWindowPosition(350, 50);
  glutCreateWindow("Smart Goku");

  initBoard();
  initMap("Mapa01.txt");
  initAgent(NULL);
  initDragonballs(NULL);
  initGL();

  goku_search(26, 15, 30, 15);

  glutMainLoop();

	return OK;
}

