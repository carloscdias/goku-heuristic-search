#include <stdlib.h>
#include <stdio.h>
#include <smartgoku.h>
#ifdef __APPLE__ && __MACH__
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

byte MAP[MAP_SIZE][MAP_SIZE];

char text[TEXT_BUFFER];

unsigned int currentTotalCost = 0;

// Main function
int main(int argc, char *argv[], char *envp[]) {	
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(630, 630);
  glutInitWindowPosition(350, 50);
  glutCreateWindow("Smart Goku");
  glutDisplayFunc(display);
  glutKeyboardFunc(controls);
  glutSpecialFunc(controls);

  initMap("Mapa01.txt");
  initAgent(NULL);
  initDragonballs(NULL);
  initGL();

  glutMainLoop();

	return OK;
}

