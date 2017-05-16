#ifndef _SMART_GOKU_H
#define _SMART_GOKU_H

#define MAP_SIZE              42
#define DRAGONBALLS_NUMBER    7
#define DRAGON_RADAR_DISTANCE 3
#define TEXT_BUFFER           50

#define GRASS_COST            1
#define WATER_COST            10
#define MOUNTAIN_COST         60

typedef unsigned char byte;

typedef struct {
  byte x, y;
} Position2D;

typedef struct {
  byte showGrid, showDragonballs, showInfo;
  unsigned int currentTotalCost;
  Position2D lastPosition;
} BoardSetup;

extern byte MAP[MAP_SIZE][MAP_SIZE];
extern Position2D Goku;
extern Position2D Dragonballs[DRAGONBALLS_NUMBER];
extern char text[TEXT_BUFFER];
extern BoardSetup board;

void initBoard();
void initMap(char *);
void initAgent(Position2D *);
void initDragonballs(Position2D **);

void moveLeft();
void moveRight();
void moveUp();
void moveDown();

void initGL();
void display();
void controls(int, int, int);
void configs(unsigned char, int, int);

#endif
