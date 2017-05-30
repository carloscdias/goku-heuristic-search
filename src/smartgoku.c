#include <stdlib.h>
#include <stdio.h>
#include <ghsdata.h>
#include <smartgoku.h>
#include <ghsgraphics.h>
#include <getopt.h>

// Init global vars
game_t game;
char info[TEXT_BUFFER];

// Main function
int main(int argc, char *argv[], char *envp[]) {	
  char c;
  int opts_index;
  // Program options
  struct option opts[] = {
    {"map",         required_argument,  0,  'm'},
    {"dragonballs", required_argument,  0,  'd'},
    {"explore",     required_argument,  0,  'e'},
    {0,             0,                  0,   0 }
  };

  // Parse arguments
  while ((c = getopt_long(argc, argv, "m:d:e:", opts, &opts_index)) != -1) {
    switch (c)
    {
      case 'm':
        printf("Sets the map filename here.\n");
        break;
      case 'd':
        printf("Sets dragonballs position here.\n");
        break;
      case 'e':
        printf("Sets explore mode here.\n");
        break;
      case '?':
        printf("Dafuq happened???.\n");
        break;
      default:
        printf("Unknown character received from getopt: %c\n", c);
        exit(EXIT_FAILURE);
        break;
    }
  }

  // Init openGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(630, 630);
  glutInitWindowPosition(350, 50);
  glutCreateWindow("Goku Heuristic Search");

  // Init environment
  init_game("Mapa01.txt", NULL);
  init_opengl();

  // OpenGL loop
  glutMainLoop();

	return EXIT_SUCCESS;
}

