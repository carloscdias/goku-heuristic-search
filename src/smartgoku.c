#include <stdlib.h>
#include <smartgoku.h>
#include <GL/gl.h>
#include <GL/freeglut.h>

#define OK  0 

// Init global vars
Position2D Goku;
Position2D Dragonballs[DRAGONBALLS_NUMBER];

byte MAP[MAP_SIZE][MAP_SIZE];

// Main function
int main(int argc, char *argv[], char *envp[]) {	
  initAgent(NULL);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Smart Goku");
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMainLoop();

	return OK;
}

