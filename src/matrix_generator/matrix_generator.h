#ifndef MG
#define MG

#include "../index.h"
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>

#define DIMX NUM_OF_CHORDS
#define DIMY NUM_OF_CHORDS + 1

char **initialize_chord_set(int HEIGHT, int WIDTH);
void read_chords(FILE *fp, char **chord_set);
char **build_chord_table(FILE *fp);
int find_chord_id(char **chord_set, char *chord);
void write_matrix(char **chord_set, gsl_matrix *transition_matrix, FILE *tr_matrix);
void btm_occurences(FILE *fp, gsl_matrix *transition_matrix, char **chord_set);
double *btm_sums(gsl_matrix *transition_matrix);
void btm_probability(gsl_matrix *transition_matrix, double *sums);
void build_transition_matrix(FILE *fp, FILE *tr_matrix, FILE *sd);
void linear_transformation(gsl_matrix *transition_matrix);
gsl_vector *QR_calculation(gsl_matrix *transition_matrix);
gsl_vector *calculate_stationary_distribution(gsl_matrix *transition_matrix);

#endif