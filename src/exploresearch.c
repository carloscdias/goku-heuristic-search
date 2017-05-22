#include <stdlib.h>
#include <utils.h>
#include <pathsearch.h>

// Explore the map problem
Problem exploreMapProblem = {&initial_state, explored_the_map, movements_on_map};

// Possível heurística para exploração do mapa: número de "pixels" não explorados
// armazenar no estado a posição e mapa explorado até o momento.
// Talvez compense modificar o formato da função de cálculo de heurística para permitir
// a passagem do nó como parâmetro para cálculo do mapa visível em tempo de execução para
// um dado nó.

void print_position(Position2D *position) {
  printf("Position: (%d, %d)\n", position->x, position->y);
}

// Explore map search
Stack explore_map_search() {
  initial_state = create_position(19, 22);

  exploreMapProblem.initial_state = initial_state;

  return (Stack)A_star_search(&exploreMapProblem, explore_map_heuristic, compare_positions, make_path);
}

// Goal for the explore the map
byte explored_the_map(Node *node) {
  Node *tmp;
  byte currentExploredMap[MAP_SIZE][MAP_SIZE];

  init_explored_map(currentExploredMap);

  tmp = node;

  while(tmp != NULL) {
    fill_explored_map(currentExploredMap, (Position2D*)tmp->state);
    tmp = tmp->parent;
  }

  printf("RESTANTE: %d\n",get_number_unexplored_regions(currentExploredMap));

  return (get_number_unexplored_regions(currentExploredMap) < 200);
}

// Number of unexplored regions
int get_number_unexplored_regions(byte currentMap[MAP_SIZE][MAP_SIZE]) {
  int x, y;
  int sum;

  sum = 0;

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      if(currentMap[x][y] == 0) {
        sum += 1;
      }
    }
  }

  return sum;
}

// Init unexplored map
void init_explored_map(byte currentMap[MAP_SIZE][MAP_SIZE]) {
  byte x, y;

  for(x = 0; x < MAP_SIZE; x++) {
    for(y = 0; y < MAP_SIZE; y++) {
      currentMap[x][y] = 0;
    }
  }
}

// Fill explored map based on position
void fill_explored_map(byte currentMap[MAP_SIZE][MAP_SIZE], Position2D *position) {
  int x, y, startX, startY, endX, endY;

  startX = position->x - DRAGON_RADAR_DISTANCE;
  startX = (startX < 0)?0:startX;
  endX = position->x + DRAGON_RADAR_DISTANCE + 1;
  endX = (endX < MAP_SIZE)?endX:(MAP_SIZE - 1);

  startY = position->y - DRAGON_RADAR_DISTANCE;
  startY = (startY < 0)?0:startY;
  endY = position->y + DRAGON_RADAR_DISTANCE + 1;
  endY = (endY < MAP_SIZE)?endY:(MAP_SIZE - 1);

  for(x = startX; x <= endX; x++) {
    for(y = startY; y <= endY; y++) {
      currentMap[x][y] = 1;
    }
  }
}

// Explore map heuristic
double explore_map_heuristic(Node *node) {
  Node *tmp;
  byte currentExploredMap[MAP_SIZE][MAP_SIZE];
  double i;

  init_explored_map(currentExploredMap);

  tmp = node;
  print_position((Position2D*)node->state);

  while(tmp != NULL) {
    fill_explored_map(currentExploredMap, (Position2D*)tmp->state);
    tmp = tmp->parent;
  }
  
  i = get_number_unexplored_regions(currentExploredMap);
  printf("current number of unexplored regions: %f\n", i);

  return i;
}

