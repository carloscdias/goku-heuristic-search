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
  initMap("Mapa01.txt");

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(630, 630);
  glutInitWindowPosition(350, 50);
  glutCreateWindow("Smart Goku");
  glutDisplayFunc(display);

  // Initialization
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, MAP_SIZE, 0, MAP_SIZE);

  //glutReshapeFunc(reshape);
  glutMainLoop();

	return OK;
}

