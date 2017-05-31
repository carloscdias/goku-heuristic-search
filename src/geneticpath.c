#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ghsdata.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <pathsearch.h>
#include <pthread.h>
#include <smartgoku.h>

#define NUMBER_OF_ARGUMENTS           2

#define DEFAULT_ELITISM_NUMBER        5
#define DEFAULT_MUTATION_PERCENTAGE   5

// Declare global variable to eliminate warnings
game_t game;

// Definitions
typedef struct {
  unsigned int individual_index, fitness, individual_length;
  position2d_t *individual;
} fitness_t;

typedef struct {
  unsigned int path_cost;
  byte_t map[MAP_SIZE][MAP_SIZE];
} explore_path_t;

// Global variables
volatile sig_atomic_t exit_loop = 0;

// Create memory for population
position2d_t
**create_population (int genes_length, int population_length)
{
  position2d_t **population;
  int i;

  population = (position2d_t**) calloc(population_length, sizeof(position2d_t*));

  for (i = 0; i < population_length; i++) {
    population[i] = (position2d_t*) calloc(genes_length, sizeof(position2d_t));
  }

  return population;
}

// Load from file function
void
load_data_from_file (char *filename, int *generation, int *population_length, int *genes_length, position2d_t ***population)
{
  int i;
  FILE *file;

  file = fopen(filename, "rb");

  // Check if opened
  if (file == NULL) {
    printf("Error opening file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // read generation
  if (fread(generation, sizeof(int), 1, file) != 1) {
    printf("Error reading generation from file %s.\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  printf("\n\nLoading generation %d from file %s...\n", *generation, filename);

  // read population
  if (fread(population_length, sizeof(int), 1, file) != 1) {
    printf("Error reading population from file %s.\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  printf("Setting number of individuals per generation as %d.\n", *population_length);

  // read genes length
  if (fread(genes_length, sizeof(int), 1, file) != 1) {
    printf("Error reading genes length from file %s.\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  printf("Setting number of genes per individual as %d.\n", *genes_length);

  // Init population
  *population = create_population(*genes_length, *population_length);

  for (i = 0; i < *population_length; i++) {
    fread((*population)[i], sizeof(position2d_t), *genes_length, file);
  }

  fclose(file);
}

// Write data to file
void
write_data_to_file (char *filename, int *generation, int *population_length, int *genes_length, position2d_t **population)
{
  int i;
  FILE *file;

  file = fopen(filename, "wb");

  // Check if opened
  if (file == NULL) {
    printf("Error opening file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // writing generation
  if (fwrite(generation, sizeof(int), 1, file) != 1) {
    printf("Error writing generation to file %s.\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  // writing population
  if (fwrite(population_length, sizeof(int), 1, file) != 1) {
    printf("Error writing population to file %s.\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  // writing genes length
  if (fwrite(genes_length, sizeof(int), 1, file) != 1) {
    printf("Error writing genes length to file %s.\n", filename);
    fclose(file);
    exit(EXIT_FAILURE);
  }

  // Write population
  for (i = 0; i < *population_length; i++) {
    fwrite(population[i], sizeof(position2d_t), *genes_length, file);
  }

  fclose(file);
}

// Write data to file
void
write_individual_to_file (char *filename, int genes_length, position2d_t *individual)
{
  FILE *file;

  file = fopen(filename, "wb");

  // Check if opened
  if (file == NULL) {
    printf("Error opening file %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // Write individual
  fwrite(individual, sizeof(position2d_t), genes_length, file);

  fclose(file);
}

// Deallocate memory for population
void
destroy_population (int population_length, position2d_t **population)
{
  int i;

  for (i = 0; i < population_length; i++) {
    free(population[i]);
  }

  free(population);
}

// Create first generation
void
create_first_generation (int population_length, int genes_length, position2d_t ***population)
{
  int i, j;

  // Init population
  *population = create_population(genes_length, population_length);

  // Randomize data for all individuals
  for (i = 0; i < population_length; i++) {
    for (j = 0; j < genes_length; j++) {
      (*population)[i][j].x = (byte_t)(rand() % MAP_SIZE);
      (*population)[i][j].y = (byte_t)(rand() % MAP_SIZE);
    }
  }
}

// Print an individual
void
print_individual (int genes_length, position2d_t *individual)
{
  int i;

  printf("(%2d, %2d)", individual[0].x, individual[0].y);

  for (i = 1; i < genes_length; i++) {
    printf("->(%2d, %2d)", individual[i].x, individual[i].y);
  }

  printf("\n");
}

// Print entire population
void
print_population (int generation, int population_length, int genes_length, position2d_t **population)
{
  int i;

  printf("\nPopulation for %d generation...\n", generation);

  for (i = 0; i < population_length; i++) {
    printf("%5d:\n", i + 1);
    print_individual(genes_length, population[i]);
    printf("\n");
  }

  printf("\n--------------------\n");
}

// Function to calculate path cost and explore rate from a search solution
void
*calculate_single_path(node_t *solution)
{
  explore_path_t *single_path;

  single_path = (explore_path_t*) malloc(sizeof(explore_path_t));
  
  // init variable
  single_path->path_cost = solution->path_cost;
  init_explored_map(single_path->map);

  while (solution != NULL) {
    fill_explored_map(single_path->map, (position2d_t*) solution->state);
    solution = solution->parent;
  }

  return ((void*) single_path);
}

// Count number of explored regions
unsigned int
count_explored_regions (const byte_t explored_map[MAP_SIZE][MAP_SIZE])
{
  byte_t x, y;
  unsigned int explored_regions = 0;

  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      if (explored_map[x][y] == EXPLORED) {
        explored_regions++;
      }
    }
  }

  return explored_regions;
}

// Add explored regions
void
add_explored_regions (byte_t current_map[MAP_SIZE][MAP_SIZE], const byte_t explored_map[MAP_SIZE][MAP_SIZE])
{
  byte_t x, y;

  for (x = 0; x < MAP_SIZE; x++) {
    for (y = 0; y < MAP_SIZE; y++) {
      if (explored_map[x][y] == EXPLORED) {
        current_map[x][y] = EXPLORED;
      }
    }
  }
}

// individual evaluation function
void
*individual_evaluation(void *params)
{
  unsigned int i, path_cost, explore_rate;
  byte_t last_x, last_y;
  byte_t explored_map[MAP_SIZE][MAP_SIZE];
  explore_path_t *single_path;
  fitness_t *individual;

  individual = (fitness_t*) params;

  // init
  init_explored_map(explored_map);
  path_cost = 0;

  // initial goku position
  last_x = 19;
  last_y = 22;

  for (i = 0; i < individual->individual_length; i++) {
    single_path = ps_generic_path_search(last_x, last_y,
        individual->individual[i].x, individual->individual[i].y, calculate_single_path);
    // Add calculated values
    path_cost += single_path->path_cost;
    add_explored_regions(explored_map, single_path->map);
    // new path begining
    last_x = individual->individual[i].x;
    last_y = individual->individual[i].y;
    // Deallocate memory
    free(single_path);
  }

  explore_rate = count_explored_regions(explored_map);

  // Fitness function
  individual->fitness = explore_rate*explore_rate - path_cost;

  return NULL;
}

// Function to compare two fitness
int
compare_fitness(const void *f1, const void *f2)
{
  fitness_t *fit1, *fit2;

  fit1 = (fitness_t*) f1;
  fit2 = (fitness_t*) f2;

  return (fit2->fitness - fit1->fitness);
}

// function to evaluate a population
void
evaluate(position2d_t **population, const int population_length, int genes_length, fitness_t *evaluation)
{
  unsigned int i;
  pthread_t tid[population_length];

  // set each individual properties in the fitness array
  // then, order that array based first in the explore, second by path cost

  for (i = 0; i < population_length; i++) {
    // individual_index, explore_fitness, path_cost_fitness, individual_length, *individual
    evaluation[i] = (fitness_t) {i, 0, genes_length, population[i]};
    // launch thread
    pthread_create(&tid[i], NULL, individual_evaluation, &(evaluation[i]));
  }

  // Wait till every thread on this generation finish
  for (i = 0; i < population_length; i++) {
    pthread_join(tid[i], NULL);
  }

  // Order the array
  qsort(evaluation, population_length, sizeof(fitness_t), compare_fitness);
  for (i = 0; i < population_length; i++) {
    printf("Evaluation for %d: %d\n", i + 1, evaluation[i].fitness);
  }

  printf("Best individual till now...\n");

  print_individual(genes_length, population[evaluation[0].individual_index]);
}

// function to generate a new population for the next generation
void
next_generation(position2d_t ***population, int population_length, int genes_length, fitness_t *evaluation)
{
  // first, remember to not change first ELITISM_NUMBER individuals of the evaluation array
  sleep(1);
  printf("Doing something...\n");
}

// Signal handler
void
signal_handler (int signo)
{
  if (signo == SIGINT) {
    exit_loop = 1;
  }
}

// Main function
int
main (int argc, char *argv[])
{
  char c;
  int opts_index, generation = 0, genes_length = 0, population_length = 0, elitism = DEFAULT_ELITISM_NUMBER;
  float mutation = DEFAULT_MUTATION_PERCENTAGE;
  position2d_t **population;
  fitness_t *evaluation;
  char *filename, *mapfile = NULL, *output_best_individual = NULL;
  time_t t;
  struct option opts[] = {
    {"genes",       required_argument,  0,  'g'},
    {"population",  required_argument,  0,  'p'},
    {"elitism",     required_argument,  0,  'e'},
    {"mutation",    required_argument,  0,  'm'},
    {"output",      required_argument,  0,  'o'},
    {0,             0,                  0,   0 }
  };

  // Register SIGINT receiver
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    printf("Problem registering callback for signal...\n");
  }

  // Initializes rand
  srand((unsigned) time(&t));

  // Parse arguments
  while ((c = getopt_long(argc, argv, "p:g:e:m:o:", opts, &opts_index)) != -1) {
    switch (c)
    {
      case 'p':
        population_length = atoi(optarg);
        break;
      case 'g':
        genes_length = atoi(optarg);
        break;
      case 'e':
        elitism = atoi(optarg);
        break;
      case 'm':
        mutation = atof(optarg);
        break;
      case 'o':
        output_best_individual = optarg;
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

  // Check for required argument
  if ((argc - optind) < NUMBER_OF_ARGUMENTS) {
    printf("Usage: %s [-p population_number] [-g genes_number] [-e elitism_number] [-m mutation_percentage] mapfile filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Set map
  mapfile = argv[optind];
  // Set file name
  filename = argv[optind + 1];
  
  // Init mapfile
  init_map(mapfile);

  // Check if file exists
  if (access(filename, F_OK) == 0) {
    // if so, load data from file
    load_data_from_file(filename, &generation, &population_length, &genes_length, &population);
    print_population(generation, population_length, genes_length, population);
  } else {
    // Create first generation
    printf("Creating first generation...\n");
    generation = 1;
    create_first_generation(population_length, genes_length, &population);

    // Print population
    print_population(generation, population_length, genes_length, population);
  }

  // Allocate memory fo evaluation array
  evaluation = (fitness_t*) calloc(population_length, sizeof(fitness_t));

  // Do the dirty work...
  // Do stuff here!!!
  while (exit_loop != 1) {
    // Here we have to evaluate the generated population
    evaluate(population, population_length, genes_length, evaluation);
    // based on the evaluation performed on the fitness process
    // decide wheter individuals should be parents of the new generation
    // select a few for elitism purposes
    // generate childs for the next generation
    // maybe mutate some individual
    next_generation(&population, population_length, genes_length, evaluation);
    // maybe print some info about the process...
    // repeat the process
  }

  // Save generated population to file
  printf("Saving generation %d to file %s...\n", generation, filename);
  write_data_to_file(filename, &generation, &population_length, &genes_length, population);

  if (output_best_individual != NULL) {
    printf("Saving current best individual as %s\n", output_best_individual);
    write_individual_to_file(output_best_individual, genes_length, population[evaluation[0].individual_index]);
  }

  // Deallocate memory for population
  free(evaluation);
  destroy_population(population_length, population);

  // Exit cleanly
  exit(EXIT_SUCCESS);
}
