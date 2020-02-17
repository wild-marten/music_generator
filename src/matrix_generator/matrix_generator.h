#ifndef MG
#define MG

#include "../index.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

#define DIMX NUM_OF_CHORDS
#define DIMY NUM_OF_CHORDS

void *clean(void *ptr);
void **clean_matrix(void **tab, int rows);
char **build_chord_table(FILE *fp);
int find_chord_id(char **chord_set, char *chord);
void write_matrix(char **chord_set, gsl_matrix *transition_matrix, FILE *result);
void btm_occurences(FILE *fp, gsl_matrix *transition_matrix, char **chord_set);
void build_transition_matrix(FILE *fp, FILE *result);

#endif