#ifndef _SMART_GOKU_H_
#define _SMART_GOKU_H_

#include <utils.h>

#define TEXT_BUFFER           50
#define REFRESH_RATE          30
#define GOKU_SEARCH_VELOCITY  300

typedef enum {IDLE, START} Status;

typedef struct {
  byte showGrid, showDragonballs, showInfo, showDragonRadar, caughtDragonballs;
  Status status;
  unsigned int currentTotalCost;
} BoardSetup;

extern char text[TEXT_BUFFER];
extern BoardSetup board;

void initGL();
void initBoard();
void initMap(char *);
void initAgent(Position2D *);
void initDragonballs(Position2D **);

void checkSeenDragonballs();

void moveGokuLeft();
void moveGokuRight();
void moveGokuUp();
void moveGokuDown();

#endif
