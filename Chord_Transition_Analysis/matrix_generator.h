#include "index.h"
#define NUM_OF_CHORDS 775
#define CHORD_LENGHT 100

void *clean(void *ptr);
void **clean_matrix(void **tab, int columns);
char **build_chord_table(FILE *fp);
int **initialize_transition_matrix();
int find_chord_id(char **chord_set, char *chord);
void print_matrix(char **chord_set, int **transition_matrix);
void write_matrix(char **chord_set, int **transition_matrix, FILE *result);
int gcd(int a, int b);
void btm_occurences(FILE *fp, int **transition_matrix, char **chord_set);
void btm_prefix_sums(int **transition_matrix);
void build_transition_matrix(FILE *fp, FILE *result);