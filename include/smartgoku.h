#ifndef _SMART_GOKU_H_
#define _SMART_GOKU_H_

#include <ghsdata.h>

// Constants for visual elements like
// text buffer, refresh rate and
// agent update rate
#define TEXT_BUFFER           50
#define REFRESH_RATE          30
#define GOKU_SEARCH_VELOCITY  300

// Global text buffer
extern char info[TEXT_BUFFER];

// Init global variable
void init_game(char*, char*);

// Performs the search algorithm
void search();

// Restart the game
void restart();

// Check if any dragonball was seen in the last movement
void check_seen_dragonballs();

// Agent movements
// Move agent up one position
void move_goku_up();

// Move agent down one position
void move_goku_down();

// Move agent left one position
void move_goku_left();

// Move agent right one position
void move_goku_right();

#endif
