#include <stdlib.h>
#include <stdio.h>
#include <ghsdata.h>
#include <smartgoku.h>
#include <ghsgraphics.h>
#include <getopt.h>
#include <pathexploresearch.h>

// Init global vars
game_t game;
char info[TEXT_BUFFER];

// Main function
int main(int argc, char *argv[], char *envp[]) {	
  char c;
  int opts_index;
  char *mapfile;
  // Program options
  struct option opts[] = {
    {"map",         required_argument,  0,  'm'},
    {"dragonballs", required_argument,  0,  'd'},
    {"path",        required_argument,  0,  'p'},
    {"rate",        no_argument,        0,  'r'},
    {0,             0,                  0,   0 }
  };

  game.dragonballs_text_positions = NULL;

  // Parse arguments
  while ((c = getopt_long(argc, argv, "m:d:p:r", opts, &opts_index)) != -1) {
    switch (c)
    {
      case 'm':
        mapfile = optarg;
        break;
      case 'd':
        game.dragonballs_text_positions = optarg;
        break;
      case 'r':
        game.explore_mode = RATE;
        break;
      case 'p':
        game.explore_mode = PATH;
        pes_init_explore(optarg);
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
  init_game(mapfile);
  init_opengl();

  // OpenGL loop
  glutMainLoop();

	return EXIT_SUCCESS;
}

