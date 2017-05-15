#ifndef _SMART_GOKU_H
#define _SMART_GOKU_H

#define MAP_SIZE              42
#define DRAGONBALLS_NUMBER    7
#define DRAGON_RADAR_DISTANCE 3

typedef unsigned char byte;

typedef struct {
  byte x, y;
} Position2D;

extern byte MAP[MAP_SIZE][MAP_SIZE];
extern Position2D Goku;
extern Position2D Dragonballs[DRAGONBALLS_NUMBER];

void initMap(char *);
void initAgent(Position2D *);
void initDragonballs(Position2D **);

void moveLeft();
void moveRight();
void moveUp();
void moveDown();

void initGL();
void display();
void reshape();

#endif
