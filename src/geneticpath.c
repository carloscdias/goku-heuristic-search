#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <ghsdata.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define NUMBER_OF_ARGUMENTS   1

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
  int opts_index, generation = 0, genes_length = 0, population_length = 0;
  position2d_t **population;
  char *filename;
  time_t t;
  struct option opts[] = {
    {"genes",       required_argument,  0,  'g'},
    {"population",  required_argument,  0,  'p'},
    {0,             0,                  0,   0 }
  };

  // Register SIGINT receiver
  if (signal(SIGINT, signal_handler) == SIG_ERR) {
    printf("Problem registering callback for signal...\n");
  }

  // Initializes rand
  srand((unsigned) time(&t));

  // Parse arguments
  while ((c = getopt_long(argc, argv, "p:g:", opts, &opts_index)) != -1) {
    switch (c)
    {
      case 'p':
        population_length = atoi(optarg);
        printf("Setting number of individuals per generation as %d.\n", population_length);
        break;
      case 'g':
        genes_length = atoi(optarg);
        printf("Setting number of genes per individual as %d\n", genes_length);
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
    printf("Usage: %s [-p population_number] [-g genes_number] filename\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // Set file name
  filename = argv[optind];

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

  // Do the dirty work...
  // Do stuff here!!!
  while (exit_loop != 1) {
    printf("Generating stuff...");
  }

  // Save generated population to file
  printf("Saving generation %d to file %s...\n", generation, filename);
  write_data_to_file(filename, &generation, &population_length, &genes_length, population);

  // Deallocate memory for population
  destroy_population(population_length, population);

  // Exit cleanly
  exit(EXIT_SUCCESS);
}
