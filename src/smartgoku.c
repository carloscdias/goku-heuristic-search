#include <stdlib.h>
#include <stdio.h>
#include <ghsdata.h>
#include <smartgoku.h>
#include <ghsgraphics.h>

#define OK  0 

// Init global vars
game_t game;
char info[TEXT_BUFFER];

// Main function
int main(int argc, char *argv[], char *envp[]) {	
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(630, 630);
  glutInitWindowPosition(350, 50);
  glutCreateWindow("Goku Heuristic Search");

  init_game("Mapa01.txt", NULL);
  init_opengl();

  glutMainLoop();

	return OK;
}

